#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>
#include <pugixml.hpp>
#include <future>
#include "APIClient.h"
#include "EntsoeParameterManager.h"
#include "DataStorageManager.h"
#include "DataProvider.h"

int main() {
    clock_t begin, end; 
    double time; 
    begin = clock();  
    
    //std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;
    /*
    // check if config.json exists
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
    */
    // create parameter manager object and configure a param set for the request
    EntsoeParameterManager paramManager;
    std::unordered_map<std::string, std::string> params = {
        {"docType",  paramManager.getDocumentType("Actual generation per type")},
        {"prcType",  paramManager.getProcessType("Realised")},
        {"psrType",  paramManager.getPsrType("Fossil Brown coal")},
        {"inDomain", paramManager.getInDomain("Germany")},
        {"prdStart", "202401282200"},
        {"prdEnd", "202401282230"},
    };
    
    std::vector<double> requestData;
    DataProvider Provider;
    requestData = Provider.get_data("202401282200","202401282215","Fossil Brown coal", "Generation_MW","Germany","Actual generation per type", "Realised");
    for (double& i :requestData){
        std::cout << std::to_string(i) << std::endl;
    }
    // create a first API Client 
    /*
    DataStorageManager DbManager("../database/DB_CO2Calc.db");
    auto deleteData = DbManager.deleteData("actualData");
    APIClient firstClient(baseUrl, apiKey);
    std::cout << "Requesting Data..." << std::endl;
    firstClient.get_request(params);
    */
    // parallel requests
    //std::vector<std::future<void>> futures;
    //std::vector<std::pair<std::string, std::string>> timeRanges = APIClient::split_time_range(params["prdStart"], params["prdEnd"], 4);
    //for (auto& i :timeRanges){
    //    std::cout << i.first << ", " << i.second << std::endl;
    //}
    /*
    for (const auto& tR : timeRanges) {
        std::unordered_map<std::string, std::string> threadParams = params;
        threadParams["prdStart"] = tR.first;
        threadParams["prdEnd"] = tR.second;
    
        futures.emplace_back(std::async(std::launch::async, [=]() {
            std::unordered_map<std::string, std::string> localParams = threadParams;  
            APIClient client(baseUrl, apiKey);
            client.get_request(localParams);
        }));
        
    }

    // Warten, bis alle Threads fertig sind
    for (auto& f : futures) {
        f.get();
    }
    
    */
    //
    //DataStorageManager DbManager("../database/DB_CO2Calc.db");
    //auto deleteData = DbManager.deleteData("actualData");
    //auto fetchedData = DbManager.fetchData();
    
    // Print all tuples in CSV format
    /*
    for (const auto& t : fetchedData) {
        std::cout << std::get<0>(t) << ","
                  << std::get<1>(t) << ","
                  << std::get<2>(t) << ","
                  << std::get<3>(t) << std::endl;
    }*/

    
    //testing for chrono converter
    /*
    std::string date = "202308152200";
    std::cout << date.size() << std::endl;
    std::chrono::system_clock::time_point dateTM = APIClient::string_to_chrono(date);
    std::cout << APIClient::chrono_to_string(dateTM) << std::endl;;
    */

    //your code to measure 
    end = clock(); 
    time = double(end - begin) / (double)CLOCKS_PER_SEC; 
    std::cout << "Execution Time: " << time << std::endl;
}