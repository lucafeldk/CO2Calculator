#include "DataProvider.h"
#include "APIClient.h"
#include "DataStorageManager.h"
#include  "EntsoeParameterManager.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <QCoreApplication>
#include <vector>


bool DataProvider::is_data_cached(){
    // check if data is cached
    // Work in progress for now
    return 0;
}

std::pair<std::vector<std::string>, std::vector<double>> DataProvider::get_data(const std::string& periodStart,const std::string& periodEnd,
    const std::string psrType,const std::string valType, const std::string inDomain, const std::string docType, const std::string prcType){
        std::pair<std::vector<std::string>, std::vector<double>> result;
        if (!is_data_cached()){
            // if data is not in cachce

            // check if config.json exists
            // get the baseUrl and apikey from the config.json
            std::string configPath = QCoreApplication::applicationDirPath().toStdString() + "/config.json";
            //std::cout << "Trying to open config.json at: " << configPath << std::endl;
            std::ifstream configFile(configPath);
            if (!configFile){
                 std::cerr << "Error:config.json file not found!" << std::endl;
                 return {};
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

            // setup connection to database and fetch data from data base
            std::string dbPath = QCoreApplication::applicationDirPath().toStdString() + "/database/DB_CO2Calc.db";
            DataStorageManager DbManager(dbPath);

            std::vector<std::string> listTimes = {requestParams["prdStart"]};
            std::chrono::_V2::system_clock::time_point tempChrono = APIClient::string_to_chrono(requestParams["prdStart"]);
            std::string timeString = requestParams["prdStart"];
            while (timeString != requestParams["prdEnd"] ){
                tempChrono += std::chrono::minutes(15);
                listTimes.emplace_back(APIClient::chrono_to_string(tempChrono));
            }
            //listTimes.emplace_back(requestParams["prdEnd"]);
            // request data and fetch it from database
            Client.get_request(requestParams);
            result = DbManager.fetchRealData(Client, valType);

            return result;
        }
        // else data is cached or partly cached (work in progress for now)
}
    