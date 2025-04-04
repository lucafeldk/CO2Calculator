#include "APIClient.h"
#include "DataStorageManager.h"
#include "CO2Calculator.h"
#include <cpr/cpr.h>
#include <pugixml.hpp>
#include <iostream>
#include <future>
#include <QCoreApplication>
#include <cmath> 

APIClient::APIClient(const std::string& base, const std::string& apiKey){
    //Initialising the API Client
    //Configuring examplary API Get Http adress
    baseURL = base;
    authToken = apiKey;
    documentType = "A75"; 
    processType = "A16";
    psrType = "B01";
    inDomain = "10Y1001A1001A83F";
    periodStart = "202308152200";
    periodEnd = "202308152215";
    
    create_requestUrl();
}

void APIClient::create_requestUrl(){
     // create paramater Url for request
    requestUrl = baseURL + 
        "?documentType=" +documentType + 
        "&processType=" + processType + 
        "&psrType=" + psrType + 
        "&in_Domain=" + inDomain + 
        "&periodStart=" + periodStart + 
        "&periodEnd=" + periodEnd +
        "&securityToken=" + authToken;
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

void APIClient::get_request(std::unordered_map<std::string,  std::string>& parameter){
    // configure request parameter of current object
    config_request(parameter);
    std::chrono::hours maxRequestDays = std::chrono::hours(24 * 365);
    auto duration = std::chrono::duration_cast<std::chrono::hours>(string_to_chrono(periodEnd) - string_to_chrono(periodStart));
    std::cout << "Tage requested:" << duration.count() << std::endl;
    
    // if the day limit is execceded for single request, split the request (Daylimit = 370 days)
    std::vector<std::pair<std::string, std::string>> timeRanges;
    if (duration.count()>24*365){
        double temp = static_cast<double>(duration.count())/(24*365);
        int parts =static_cast<int>(std::ceil(temp));
        std::cout << "Duration is higher then 365 days cut into " << temp << " parts" <<  std::endl;
        timeRanges = split_time_range(periodStart, periodEnd, parts);
    }
    
    // Make request to Entsoe API
    cpr::Response response;
    if (!timeRanges.empty()){
        // multiple requests
        std::string tempPeriodStart = periodStart;
        std::string tempPeriodEnd = periodEnd;
        for (std::pair<std::string, std::string> &timeRange:timeRanges){
            //single request is possible
            parameter["prdStart"] = timeRange.first;
            parameter["prdEnd"] = timeRange.second;
            config_request(parameter);
            std::cout << "Start:" << periodStart << std::endl;
            std::cout << "End: " << periodEnd << std::endl;
            response = cpr::Get(
                cpr::Url{requestUrl},
                cpr::Header{{"Accept-Encoding", "gzip"}}
            );
            // Error handling for the get request
            if (response.error) {
                throw std::runtime_error("Network error: " + response.error.message);
            }

            if (response.status_code != 200) {
                throw std::runtime_error("HTTP error: Status code " + std::to_string(response.status_code) +
                                 " - " + response.text);
            }
            // call xml_parser to handle the xml response message
            xml_parser(response);
            }
        parameter["prdStart"] = tempPeriodStart;
        parameter["prdEnd"] = tempPeriodEnd;
        config_request(parameter);
    }
    else{
        //single request is possible
        response = cpr::Get(
            cpr::Url{requestUrl},
            cpr::Header{{"Accept-Encoding", "gzip"}}
        );

        // Error handling for the get request
        if (response.error) {
            throw std::runtime_error("Network error: " + response.error.message);
        }

        if (response.status_code != 200) {
            throw std::runtime_error("HTTP error: Status code " + std::to_string(response.status_code) +
                                 " - " + response.text);
        }
         // call xml_parser to handle the xml response message
        xml_parser(response);
    }
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

    // specify nodes where to extract Information
    pugi::xml_node root = doc.child("GL_MarketDocument");
    pugi::xml_node period = root.child("TimeSeries").child("Period"); // in the xml period node the <point> values are stored

    //create datetime for the loop
    std::chrono::system_clock::time_point timestamp = string_to_chrono(periodStart);
    //create database manager object and insert data into db
    //endDate not included, eg 22:00-23:00, 23:00 is not included meaning data from 22:00-22:45
    std::string dbPath = QCoreApplication::applicationDirPath().toStdString() + "/database/DB_CO2Calc.db";
    std::cout << "Trying to write database at: " << dbPath << std::endl;
    DataStorageManager dbManager(dbPath);
    CO2Calculator CO2Calc;

    dbManager.beginTransaction(); //ensures batch insert
    for (pugi::xml_node point = period.child("Point"); point; point = point.next_sibling("Point")) {
        double power = point.child("quantity").text().as_double();
        double emissions = CO2Calc.calcCO2(power, psrType);
        dbManager.insertData(chrono_to_string(timestamp), inDomain, psrType, power, emissions);
        timestamp += std::chrono::minutes(15);
    }
    dbManager.commitTransaction();
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

    tm.tm_isdst = -1;  // deactivates automatic summertime detection

    std::time_t timestamp = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(timestamp);

}

std::string APIClient::chrono_to_string (const std::chrono::system_clock::time_point date) {
    // Convert chrono time back to string
    std::time_t time_t_val = std::chrono::system_clock::to_time_t(date);

    // time_t to utc timezone
    std::tm* tm_struct = std::localtime(&time_t_val);//std::gmtime(&time_t_val);

    // format the string
    std::ostringstream ss;
    ss << std::put_time(tm_struct, "%Y%m%d%H%M"); // Format YYYYMMDDHHMMSS

    return ss.str();
}

std::vector<std::pair<std::string, std::string>> APIClient::split_time_range(const std::string& start, const std::string& end, int parts){
    std::vector<std::pair<std::string, std::string>> timeIntervals;

    // Convert start- and endtime
    std::chrono::system_clock::time_point startTime = string_to_chrono(start);
    std::chrono::system_clock::time_point endTime = string_to_chrono(end);

    // calculate durations in minutes and consider the 15 minutes intervalls
    auto totalDuration = std::chrono::duration_cast<std::chrono::minutes>(endTime - startTime);
    auto baseDuration = (totalDuration.count() / parts / 15) * 15;
    auto remainingMinutes = (totalDuration.count() - (baseDuration * parts));

    auto intervalStart = startTime;

    for (int i = 0; i < parts; ++i) {
        auto intervalEnd = intervalStart + std::chrono::minutes(baseDuration);

        //if there are remaining minutes, add them to the first intervall
        if (remainingMinutes >= 15) {
            intervalEnd += std::chrono::minutes(15);
            remainingMinutes -= 15;
        }

        // makes sure that the last intervall includes the endtime
        if (i == parts - 1) {
            intervalEnd = endTime;
        }

        timeIntervals.push_back({chrono_to_string(intervalStart), chrono_to_string(intervalEnd)});
        intervalStart = intervalEnd;  // Next Intervall
    }

    return timeIntervals;
}



