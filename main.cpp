#include <cpr/cpr.h>
#include <iostream>

class APIClient{
    public: 
    std::string authToken;
    std::string baseURL;
    

    
    
};




int main() {
    // set url parameters
    std::string api_key = "here token";
    std::string base_url = "https://web-api.tp.entsoe.eu/api";
    std::string document_type = "A75";
    std::string process_type = "A16";
    std::string in_domain = "10Y1001A1001A83F"; //countrycode
    std::string period_start = "202308152200"; // starttime
    std::string period_end = "202308162200";   // endtime

    //configure request url
    std::string url = base_url + "?securityToken=" + api_key +
                      "&documentType=" + document_type +
                      "&processType=" + process_type +
                      "&in_Domain=" + in_domain +
                      "&periodStart=" + period_start +
                      "&periodEnd=" + period_end;

    //send get request to api
    cpr::Response response = cpr::Get(cpr::Url{url});

    // error handling
    if (response.error){
        std::cerr << "Error:" << response.error.message << std::endl;
        return 1;
    }

    // print response
    std::cout << "Status Code: " << response.status_code << std::endl;
    std::cout << "Antwort: " << response.text << std::endl;
    
    return 0;
}