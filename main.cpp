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
    APIClient(std::string b) : baseURL(b){}                 //Constructor
    void get_request(std::string, const std::string&);      //Api GET request method      
    //void data_parser(std::string);                          //Data parser 
};

void APIClient::get_request(std::string p, const std::string& api_key){
    paramURL = p + "&securityToken=" + api_key;
    cpr::Response response = cpr::Get(cpr::Url{baseURL + paramURL});
    if (response.error) {
        throw std::runtime_error("Network error: " + response.error.message);
    }

    if (response.status_code != 200) {
        throw std::runtime_error("HTTP error: Status code " + std::to_string(response.status_code) +
                                 " - " + response.text);
    }
        std::cout << "Status Code" << response.status_code << std::endl;
        std::cout << "Response:" << response.text << std::endl;
    
}

const std::map<std::string, std::string> psrType{
    {"A03", "Mixed"},
    {"A04", "Generation"},
    {"A05", "Load"},
    {"BO1", "Biomass"},
    {"BO2", "Fossil Brown coal"},
    {"BO3", "Fossil Coal-derived gas"},
    {"BO4", "Fossil Gas"},
    {"BO5", "Fossil Hard coal"},
    {"BO6", "Fossil Oil"},
    {"BO7", "Fossil Oil shale"},
    {"BO8", "Fossil Peat"},
    {"BO9", "Geothermal"},
    {"B10", "Hydro Pumped Storage"},
    {"B11", "Hydro Run-of-river and poundage"},
    {"B12", "Hydro Water Reservoir"},
    {"B13", "Marine"},
    {"B14", "Nuclear"},
    {"B15", "Other renewable"},
    {"B16", "Solar"},
    {"B17", "Waste"},
    {"B18", "Wind Offshore"},
    {"B19", "Wind Onshore"},
    {"B20", "Other"},
    {"B25", "Energy storage"},
};


const std::map<std::string, std::string>documentType{
    {"A01", "Day ahead"},
    {"A02", "Intra day incremental"},
    {"A16", "Realised"},
    {"A18", "Intraday total"},
    {"A31", "Week ahead"},
    {"A32", "Month ahead"},
    {"A33", "Year ahead"},
    {"A39", "Synchronisation process"},
    {"A40", "Intraday process"},
    {"A46", "Replacement reserve"},
    {"A47", "Manual frequency restoration reserve"},
    {"A51", "Automatic frequency restoration reserve"},
    {"A52", "Frequency containment reserve"},
    {"A56", "Frequency restoration reserve"},
    {"A60", "Scheduled activation mFRR"},
    {"A61", "Direct activation mFRR"},
    {"A67", "Central Selection aFRR"},
    {"A68", "Local Selection aFRR"}
};

const std::map<std::string, std::string> processTypeMap = {
    {"A01", "Day ahead"},
    {"A02", "Intra day incremental"},
    {"A09", "Finalized schedule"},
    {"A11", "Aggregated energy data report"},
    {"A15", "Acquiring system operator reserve schedule"},
    {"A16", "Realised"},
    {"A18", "Intraday total"},
    {"A24", "Bid document"},
    {"A25", "Allocation result document"},
    {"A26", "Capacity document"},
    {"A31", "Agreed capacity"},
    {"A32", "Month ahead"},
    {"A33", "Year ahead"},
    {"A37", "Reserve bid document"},
    {"A38", "Reserve allocation result document"},
    {"A39", "Synchronisation process"},
    {"A40", "Intraday process"},
    {"A44", "Price Document"},
    {"A46", "Replacement reserve"},
    {"A47", "Manual frequency restoration reserve"},
    {"A51", "Automatic frequency restoration reserve"},
    {"A52", "Frequency containment reserve"},
    {"A56", "Frequency restoration reserve"},
    {"A60", "Scheduled activation mFRR"},
    {"A61", "Estimated Net Transfer Capacity"},
    {"A63", "Redispatch notice"},
    {"A65", "System total load"},
    {"A67", "Central Selection aFRR"},
    {"A68", "Installed generation per type"},
    {"A69", "Wind and solar forecast"},
    {"A70", "Load forecast margin"},
    {"A71", "Generation forecast"},
    {"A72", "Reservoir filling information"},
    {"A73", "Actual generation"},
    {"A74", "Wind and solar generation"},
    {"A75", "Actual generation per type"},
    {"A76", "Load unavailability"},
    {"A77", "Production unavailability"},
    {"A78", "Transmission unavailability"},
    {"A79", "Offshore grid infrastructure unavailability"},
    {"A80", "Generation unavailability"},
    {"A81", "Contracted reserves"},
    {"A82", "Accepted offers"},
    {"A83", "Activated balancing quantities"},
    {"A84", "Activated balancing prices"},
    {"A85", "Imbalance prices"},
    {"A86", "Imbalance volume"},
    {"A87", "Financial situation"},
    {"A88", "Cross border balancing"},
    {"A89", "Contracted reserve prices"},
    {"A90", "Interconnection network expansion"},
    {"A91", "Counter trade notice"},
    {"A92", "Congestion costs"},
    {"A93", "DC link capacity"},
    {"A94", "Non EU allocations"},
    {"A95", "Configuration document"},
    {"B11", "Flow-based allocations"},
    {"B17", "Aggregated netted external TSO schedule document"},
    {"B45", "Bid Availability Document"}
    };

    const std::map<std::string, std::string> inDomain{
        {"Denmark", "10Y1001A1001A65H"},
        {"Germany", "10Y1001A1001A83F"}
    };

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