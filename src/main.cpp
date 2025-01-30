#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <filesystem>
#include <pugixml.hpp>
#include "APIClient.h"


int main() {
    //std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;
    std::ifstream configFile("config.json");
    if (!configFile){
        std::cerr << "Error:config.json file not found!" << std::endl;
        return 1;
    }

    nlohmann::json config;
    configFile >> config;

    // set url parameters
    std::string const api_key = config["api_key"];
    std::string const base_url = "https://web-api.tp.entsoe.eu/api";
    std::string document_type = "A75";
    std::string process_type = "A16";
    std::string psr_type = "B02";              // production type e.g. BO2 = Brown Coal
    std::string in_domain = "10Y1001A1001A83F"; //domain code: Biding Zone, Control Area or Country "10Y1001A1001A83F"
    std::string period_start = "202308152215"; // starttime
    std::string period_end = "202308152230";   // endtime

    //configure request url
    std::string param_url ="?documentType=" + document_type +
                      "&processType=" + process_type +
                      "&in_Domain=" + in_domain +
                      "&periodStart=" + period_start +
                      "&periodEnd=" + period_end +
                      "&psrType=" + psr_type;

    APIClient firstClient(base_url);
    firstClient.get_request(param_url,api_key);
}