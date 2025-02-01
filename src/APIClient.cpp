#include "APIClient.h"
#include <cpr/cpr.h>
#include <pugixml.hpp>

void APIClient::get_request(std::string& param, const std::string& api_key){
    // add api key to the requested parameters
    paramURL = param + "&securityToken=" + api_key;
    
    // api get request via cpr
    cpr::Response response = cpr::Get(cpr::Url{baseURL + paramURL});
    
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
    
    std::string documentID = root.child("mRID").text().as_string();
    std::string createdDateTime = root.child("createdDateTime").text().as_string();
    std::string startTime = root.child("time_Period.timeInterval").child("start").text().as_string();
    std::string endTime = root.child("time_Period.timeInterval").child("end").text().as_string();
    //int quantity = root.child("TimeSeries").child("Period").child("Point").child("quantity").text().as_int();

    // print out the parsed values
    std::cout << "Document ID: " << documentID << std::endl;
    std::cout << "Date: " << createdDateTime << std::endl;
    std::cout << "Time Interval: " << startTime << " - " << endTime << std::endl;

    // loop through the xml <point> messages where the <quantity> (real values) are stored
    for(pugi::xml_node point : period.children("Point")){
        int quantity = point.child("quantity").text().as_int();
        std::cout << "Quantity: " << quantity << " MAW" << std::endl;
    }
}




