#include "DataStorageManager.h"

DataStorageManager::DataStorageManager(const std::string& dbPath){

}

DataStorageManager::~DataStorageManager(){

}

bool DataStorageManager::insertData(const std::string& timestamp, const std::string& country,
                    const std::string& generationType, double generationMW){

}

std::vector<std::tuple<std::string, std::string, std::string, double>> DataStorageManager::fetchData(){

}

bool DataStorageManager::executeQuery(const std::string& query){

}