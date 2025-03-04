#include "DataProvider.h"
#include "APIClient.h"
#include "DataStorageManager.h"
#include  "EntsoeParameterManager.h"
#include <nlohmann/json.hpp>


bool DataProvider::is_data_cached(){
    // check if data is cached
}

std::vector<double> DataProvider::get_data(const std::string& periodStart,const std::string& periodEnd,
    const std::string psrType,const std::string valType, const std::string inDomain, const std::string docType, const std::string prcType){
        std::vector<double> result;
        if (!is_data_cached()){
            // if data is not in cachce

            // check if config.json exists
            // get the baseUrl and apikey from the config.json
            std::ifstream configFile("config.json");
            if (!configFile){
                 std::cerr << "Error:config.json file not found!" << std::endl;
            }
            nlohmann::json config;
            configFile >> config;
            std::string const apiKey = config["api_key"];
            std::string const baseUrl = config["base_url"];

            //create Client for request
            APIClient Client(baseUrl,apiKey);
            EntsoeParameterManager paramManager;
            std::unordered_map<std::string, std::string> requestParams = {
                {"docType",  paramManager.getDocumentType(docType)},
                {"prcType",  paramManager.getProcessType(prcType)},
                {"psrType",  paramManager.getPsrType(psrType)},
                {"inDomain", paramManager.getInDomain(inDomain)},
                {"prdStart", periodStart},
                {"prdEnd", periodEnd},
            };
            Client.get_request(requestParams);
            
            // setup connection to database and fetch data from data base
            DataStorageManager DbManager("../database/DB_CO2Calc.db");
            result = DbManager.fetchRealData(Client, "Generation_MW");

            return result;
        }
        // else data is cached

    }