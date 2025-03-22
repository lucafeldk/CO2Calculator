#include "DataStorageManager.h"
#include <sstream>


DataStorageManager::DataStorageManager(const std::string& dbPath){
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK){
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
    else{
        executeQuery("PRAGMA journal_mode=WAL;"); 
        createTable("actualData");
    }
}

DataStorageManager::~DataStorageManager(){
    if (db){
        sqlite3_close(db);
    }
}

bool DataStorageManager::insertData(const std::string& timestamp, const std::string& country,
                    const std::string& generationType, double generationMW, double emissiongkwh) {
    const char* sql = "INSERT INTO actualData (Timestamp, Country, GenerationType, Generation_MW, CO2Emissions_gCO2eq) "
                    "VALUES (?, ?, ?, ?, ?) "
                    "ON CONFLICT(Timestamp, GenerationType) DO UPDATE "
                    "SET Generation_MW = excluded.Generation_MW, CO2Emissions_gCO2eq = excluded.CO2Emissions_gCO2eq;";
  
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error (prepare): " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
  
    // Parameter setzen (Prepared Statements)
    sqlite3_bind_text(stmt, 1, timestamp.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, country.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, generationType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, generationMW);
    sqlite3_bind_double(stmt, 5, emissiongkwh);
  
    // SQL ausführen
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
  
    // Prepared Statement schließen
    sqlite3_finalize(stmt);
  
    return success;
}

bool DataStorageManager::deleteData(const std::string& tableName){
    std::string query = "DELETE FROM " + tableName;
    return executeQuery(query);
}

bool DataStorageManager::createTable(const std::string& tableName){
    std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " ("
        "Timestamp TEXT NOT NULL,"
        "Country TEXT NOT NULL DEFAULT 'Germany',"
        "GenerationType TEXT NOT NULL,"
        "Generation_MW REAL NOT NULL,"
        "CO2Emissions_gCO2eq REAL NOT NULL,"  
        "PRIMARY KEY (Timestamp, GenerationType)"
    ");";
    return executeQuery(query);
}

bool DataStorageManager::executeQuery(const std::string& query){
    // executes a sql query, false: query failed, true:query sucess entry are made in the database
    char* errorMessage = nullptr;
    if(sqlite3_exec(db, query.c_str(),nullptr, nullptr, &errorMessage) != SQLITE_OK){
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

std::vector<std::tuple<std::string, std::string, std::string, double>> DataStorageManager::fetchData(){
    std::vector<std::tuple<std::string, std::string, std::string, double>> results;
    std::string query = "SELECT Timestamp, Country, GenerationType, Generation_MW, CO2Emissions_gCO2eq FROM actualData;";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, query.c_str(), -1 ,&stmt, nullptr) == SQLITE_OK){
        while(sqlite3_step(stmt) == SQLITE_ROW){
            std::string timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string generationType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            double generationMW = sqlite3_column_double(stmt, 3);
            results.emplace_back(timestamp, country, generationType, generationMW);
        }
        sqlite3_finalize(stmt);
    }
    else{
        std::cerr << "SQL error: Unable to fetch data" << std::endl;
    }
    return results; 
}

void DataStorageManager::beginTransaction(){
    executeQuery("BEGIN TRANSACTION;");
}

void DataStorageManager::commitTransaction(){
    executeQuery("COMMIT;");
}


std::pair<std::vector<std::string>, std::vector<double>> DataStorageManager::fetchRealData(const APIClient& Client, const std::string valType) {
    std::vector<std::string> timestamps;
    std::vector<double> valueData;

    std::string query = "SELECT Timestamp, "+ valType + " FROM actualData WHERE Timestamp BETWEEN ? AND ? AND Country = ? AND GenerationType = ?;";
    sqlite3_stmt* stmt;
        
    std::string periodStart = Client.get_PeriodStart();
    std::string periodEnd = Client.get_PeriodEnd();
    std::string country = Client.get_InDomain();
    std::string generationType = Client.get_PsrType();

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
       // Bind parameters      
        sqlite3_bind_text(stmt, 1, periodStart.c_str(), -1, SQLITE_STATIC);  // Timestamp (Start)
        sqlite3_bind_text(stmt, 2, periodEnd.c_str(), -1, SQLITE_STATIC);    // Timestamp (End)
        sqlite3_bind_text(stmt, 3, country.c_str(), -1, SQLITE_STATIC);      // Country
        sqlite3_bind_text(stmt, 4, generationType.c_str(), -1, SQLITE_STATIC); // GenerationType

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            timestamps.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))); // Timestamp speichern
            valueData.push_back(sqlite3_column_double(stmt, 1)); // Generation_MW speichern
        }

        sqlite3_finalize(stmt);
    } 
    else {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    std::cout << "Fetched " << timestamps.size() << " rows." << std::endl;
    return {timestamps, valueData}; // Rückgabe als Pair von Vektoren
}

