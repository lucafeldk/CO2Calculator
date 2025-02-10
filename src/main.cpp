#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>
#include <pugixml.hpp>
#include "APIClient.h"
#include "EntsoeParameterManager.h"
#include "DataStorageManager.h"

int main() {
    std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;
    std::ifstream configFile("config.json");
    if (!configFile){
        std::cerr << "Error:config.json file not found!" << std::endl;
        return 1;
    }
    
    //get the apikey from the config.json
    nlohmann::json config;
    configFile >> config;
    std::string const apiKey = config["api_key"];

    // Current base URL of the Entsoe API
    std::string const baseUrl = "https://web-api.tp.entsoe.eu/api";

    // create parameter manager object and configure a param set for the request
    EntsoeParameterManager paramManager;
    std::unordered_map<std::string, std::string> params = {
        {"docType", "?documentType=" + paramManager.getDocumentType("Actual generation per type")},
        {"prcType", "&processType=" + paramManager.getProcessType("Realised")},
        {"psrType", "&psrType=" + paramManager.getPsrType("Fossil Brown coal")},
        {"inDomain", "&in_Domain=" + paramManager.getInDomain("Germany")},
        {"prdStart", "&periodStart=202308152200"},
        {"prdEnd", "&periodEnd=202308162200"},
    };

    // create a first API Client 
    APIClient firstClient(baseUrl, apiKey);
    firstClient.config_request(params);
    firstClient.get_request();

    //
    DataStorageManager DbManager("../database/DB_CO2Calc.db");
    auto fetchedData = DbManager.fetchData();
    
    // Print all tuples in CSV format
    for (const auto& t : fetchedData) {
        std::cout << std::get<0>(t) << ","
                  << std::get<1>(t) << ","
                  << std::get<2>(t) << ","
                  << std::get<3>(t) << std::endl;
    }

    /*
    //testing for chrono converter
    std::string date = "202308152200";
    std::cout << date.size() << std::endl;
    std::chrono::system_clock::time_point dateTM = APIClient::string_to_chrono(date);
    dateTM = dateTM + std::chrono::minutes(15);
    std::cout << APIClient::chrono_to_string(dateTM) << std::endl;;
    */
}