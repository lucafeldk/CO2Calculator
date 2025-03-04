#ifndef DATA_STORAGE_MANAGER_H
#define DATA_STORAGE_MANAGER_H

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <unordered_map>
#include "APIClient.h"
#include "EntsoeParameterManager.h"


class DataStorageManager{
    private:
    sqlite3* db;
    EntsoeParameterManager* paramManager;
    bool executeQuery(const std::string& query);
    
    public:
    DataStorageManager(const std::string& dbPath);  // Contructor
    ~DataStorageManager();                          // Destructor

    bool insertData(const std::string& timestamp, const std::string& country,
                    const std::string& generationType, double generationMW, double emissiongkwh);

    std::vector<std::tuple<std::string, std::string, std::string, double>> fetchData();
    std::vector<double> fetchRealData(const APIClient& Client, std::string valType);
    bool deleteData(const std::string& tableName);   
    bool createTable(const std::string& tableName);  
    void beginTransaction();
    void commitTransaction();                         

};


#endif