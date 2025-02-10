#include "APIClient.h"
#include "DataStorageManager.h"
#include <cpr/cpr.h>
#include <pugixml.hpp>
#include <iostream>



APIClient::APIClient(const std::string& base, const std::string& apiKey){
    //Initialising the API Client
    //Configuring examplary API Get Http adress
    baseURL = base;
    authToken = "&securityToken=" + apiKey;
    documentType = "?documentType=A75"; 
    processType = "&processType=A16";
    psrType = "&psrType=B01";
    inDomain = "&in_Domain=10Y1001A1001A83F";
    periodStart = "&periodStart=202308152200";
    periodEnd = "&periodEnd=202308152215";
    
    create_requestUrl();
}

void APIClient::create_requestUrl(){
     // create paramater Url for request
    requestUrl = baseURL + documentType + processType + psrType + inDomain + periodStart + periodEnd + authToken;
    //std::cout <<"Requested URL String: "<< requestUrl << std::endl;
}
void APIClient::config_request(std::unordered_map<std::string, std::string>& parameter){

    documentType = parameter["docType"];
    processType = parameter["prcType"];
    psrType = parameter["psrType"]; 
    inDomain = parameter["inDomain"]; 
    periodStart = parameter["prdStart"]; 
    periodEnd = parameter["prdEnd"];   

    create_requestUrl();
}

void APIClient::get_request(){
   
     // api get request via cpr
    cpr::Response response = cpr::Get(cpr::Url{requestUrl});
    
    // Error handling for the get request
    if (response.error) {
        throw std::runtime_error("Network error: " + response.error.message);
    }

    if (response.status_code != 200) {
        throw std::runtime_error("HTTP error: Status code " + std::to_string(response.status_code) +
                                 " - " + response.text);
    }

    //Print out Information from the response
    std::cout << "Status Code: " << response.status_code << std::endl;
    //std::cout << "Response:" << response.text << std::endl;
    
    // call xml_parser to handle the xml response message
    xml_parser(response);

}

void APIClient::xml_parser(cpr::Response& response){
        //read in xml response 
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(response.text.c_str());
    
    //in case of empty result throw parsing error
    if (!result){
        std::cerr << "XML Parsing Error: " << result.description() << std::endl;
        return;
    };

    // extract the Information from the xml response
    pugi::xml_node root = doc.child("GL_MarketDocument");
    pugi::xml_node period = root.child("TimeSeries").child("Period"); // in the xml period node the <point> values are stored

    std::string createdDateTime = root.child("createdDateTime").text().as_string();
    std::string startTime = root.child("time_Period.timeInterval").child("start").text().as_string();
    std::string endTime = root.child("time_Period.timeInterval").child("end").text().as_string();
    std::string generationType = root.child("TimeSeries").child("MktPSRType").child("psrType").text().as_string();
    std::string country = "Germany"; // Standardwert, da dein Schema das so vorsieht


    //create datetime for the loop
    std::chrono::system_clock::time_point timestamp = string_to_chrono(periodStart.erase(0,13));
    
    // create database manager object and insert data into db
    DataStorageManager dbManager("../database/DB_CO2Calc.db");
    for (pugi::xml_node point : period.children("Point")) {
        int quantity = point.child("quantity").text().as_int();
        dbManager.insertData(chrono_to_string(timestamp), country, generationType, quantity);
        timestamp += std::chrono::minutes(15);
    }

    // loop through the xml <point> messages where the <quantity> (real values) are stored
    /*
    for(pugi::xml_node point : period.children("Point")){
        int quantity = point.child("quantity").text().as_int();
        std::cout << "Quantity: " << Quantity << " MW" << std::endl;
    }
    */
}

std::chrono::system_clock::time_point APIClient::string_to_chrono(const std::string& date){
   
    if (date.size() != 12) { // checks if the date has the correct size
        throw std::runtime_error("Ungültiges Datumsformat: " + date);
    }

    std::tm tm = {};
    tm.tm_year = std::stoi(date.substr(0, 4)) - 1900; // Year 
    tm.tm_mon  = std::stoi(date.substr(4, 2)) - 1;    // month
    tm.tm_mday = std::stoi(date.substr(6, 2));        // day
    tm.tm_hour = std::stoi(date.substr(8, 2));        // hour
    tm.tm_min  = std::stoi(date.substr(10, 2));       // minute
    tm.tm_sec  = 0; // seconds always 0

    std::time_t timestamp = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(timestamp);

}

std::string APIClient::chrono_to_string (const std::chrono::system_clock::time_point date) {
    // Convert chrono time back to string
    std::time_t time_t_val = std::chrono::system_clock::to_time_t(date);

    // time_t to struct tm local time
    std::tm* tm_struct = std::localtime(&time_t_val);

    // format the string
    std::ostringstream ss;
    ss << std::put_time(tm_struct, "%Y%m%d%H%M"); // Format YYYYMMDDHHMMSS

    return ss.str();
}



