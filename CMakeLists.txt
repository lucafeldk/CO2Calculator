cmake_minimum_required(VERSION 3.20)
project(CO2Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Backend hinzufügen
add_subdirectory(backend)

# Frontend hinzufügen
add_subdirectory(frontend)
