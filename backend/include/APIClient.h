#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <iostream>
#include <cpr/cpr.h>
#include <unordered_map>

class APIClient{
    private:
        std::string authToken;
        std::string psrType;
        std::string documentType;
        std::string processType;
        std::string inDomain;
        std::string periodStart;
        std::string periodEnd;
        std::string baseURL;
        std::string requestUrl;
    
    public:
        APIClient(const std::string& base, const std::string& apiKey);                                                                          //Constructor
        void config_request(std::unordered_map<std::string,  std::string>& parameter);                                                          //config method to configure the request
        void create_requestUrl();
        void get_request(std::unordered_map<std::string,  std::string>& parameter);                                                             //API get Request method
        void xml_parser(cpr::Response& response);                                                                                               //Data parser for xml 
        static std::chrono::system_clock::time_point string_to_chrono(const std::string& date);                                    
        static std::string chrono_to_string(const std::chrono::system_clock::time_point date);
        static std::vector<std::pair<std::string, std::string>> split_time_range(const std::string& start, const std::string& end, int parts); //spliting into time ranges

        // Get methods
        std::string get_DocumentType() const { return documentType; }
        std::string get_ProcessType() const { return processType; }
        std::string get_PsrType() const { return psrType; }
        std::string get_InDomain() const { return inDomain; }
        std::string get_PeriodStart() const { return periodStart; }
        std::string get_PeriodEnd() const { return periodEnd; }
};

#endif