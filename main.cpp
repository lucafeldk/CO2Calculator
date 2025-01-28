#include <fstream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <iostream>
#include <filesystem>

class APIClient{
    private:
    std::string authToken;
    std::string baseURL;
    std::string paramURL;

    public: 
    APIClient(std::string b) : baseURL(b){}
    void get_request(std::string, const std::string&);      
};

void APIClient::get_request(std::string p, const std::string& api_key){
    paramURL = p + "&securityToken=" + api_key;
    cpr::Response response = cpr::Get(cpr::Url{baseURL + paramURL});
    if (response.error){
        std::cerr << "Error:" << response.error.message << std::endl;   
    }
    else{
        std::cout << "Status Code" << response.status_code << std::endl;
        std::cout << "Response:" << response.text << std::endl;
    }
}

int main() {
    // set url parameters
    //std::cout << "Current Working Directory: " << std::filesystem::current_path() << std::endl;
    

    std::ifstream configFile("config.json");
    if (!configFile){
        std::cerr << "Error:config.json file not found!" << std::endl;
        return 1;
    }

    nlohmann::json config;
    configFile >> config;

    std::string const api_key = config["api_key"];
    std::string const base_url = "https://web-api.tp.entsoe.eu/api";
    std::string document_type = "A75";
    std::string process_type = "A16";
    std::string in_domain = "10Y1001A1001A83F"; //countrycodeS
    std::string period_start = "202308152215"; // starttime
    std::string period_end = "202308152230";   // endtime

    //configure request url
    std::string param_url ="?documentType=" + document_type +
                      "&processType=" + process_type +
                      "&in_Domain=" + in_domain +
                      "&periodStart=" + period_start +
                      "&periodEnd=" + period_end;

    APIClient firstClient(base_url);
    firstClient.get_request(param_url,api_key);
}