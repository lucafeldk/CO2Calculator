#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <iostream>

class APIClient{
    private:
        std::string authToken;
        std::string baseURL;
        std::string paramURL;
    
    public:
        APIClient(const std::string& base) : baseURL(base){};                 //Constructor
        void get_request(std::string&  , const std::string &key);  //API get Request
        void data_parser(std::string);                                  //Data parser 
};

#endif