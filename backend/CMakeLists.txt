cmake_minimum_required(VERSION 3.20)
project(CO2Calculator_Backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Backend als statische Bibliothek bauen
add_library(co2_backend STATIC
    src/APIClient.cpp
    src/EntsoeParameterManager.cpp
    src/DataStorageManager.cpp
    src/CO2Calculator.cpp
)

# Externe Abhängigkeiten einbinden
include(FetchContent)

FetchContent_Declare(
    cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.11.0
)
FetchContent_MakeAvailable(cpr)
target_link_libraries(co2_backend PRIVATE cpr::cpr)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)
target_link_libraries(co2_backend PRIVATE nlohmann_json::nlohmann_json)

FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.13
)
FetchContent_MakeAvailable(pugixml)
target_link_libraries(co2_backend PRIVATE pugixml)

# SQLite3-Pfade setzen
set(SQLite3_INCLUDE_DIR "D:/Programme/MSYS2/mingw64/include")
set(SQLite3_LIBRARY "D:/Programme/MSYS2/mingw64/lib/libsqlite3.a")

include_directories(${SQLite3_INCLUDE_DIR})
link_directories("D:/Programme/MSYS2/mingw64/lib")
target_link_libraries(co2_backend PRIVATE ${SQLite3_LIBRARY})
