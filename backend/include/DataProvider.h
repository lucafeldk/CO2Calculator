#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <unordered_map>
#include <iostream>
#include <vector>

class DataProvider{
    private:
        std::unordered_map<std::string, std::vector<double>> currentData; // cache for current data
        
    public:
        bool is_data_cached(); // check if data is already cached
        std::vector <double> get_data(const std::string& periodStart, const std::string& periodEnd,
             const std::string psrType, const std::string valType, const std::string inDomain, const std::string docType, const std::string prcType); // gets Data from db or cache
};

#endif