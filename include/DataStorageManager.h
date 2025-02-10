#ifndef DATA_STORAGE_MANAGER_H
#define DATA_STORAGE_MANAGER_H

#include <iostream>
#include <sqlite3.h>
#include <vector>


class DataStorageManager{
    private:
    sqlite3* db;
    bool executeQuery(const std::string& query);
    
    public:
    DataStorageManager(const std::string& dbPath);  // Contructor
    ~DataStorageManager();                          // Destructor

    bool insertData(const std::string& timestamp, const std::string& country,
                    const std::string& generationType, double generationMW);

    std::vector<std::tuple<std::string, std::string, std::string, double>> fetchData();
    bool deleteData(const std::string& tableName);   
    bool createTable(const std::string& tableName);                           

};


#endif