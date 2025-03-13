cmake_minimum_required(VERSION 3.16)

project(CO2Calculator_Frontend VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt sicherstellen
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets PrintSupport)

# Include-Verzeichnisse setzen
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/backend/include)

# QCustomPlot als statische Bibliothek
add_library(qcustomplot STATIC qcustomplot.cpp)
target_link_libraries(qcustomplot PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::PrintSupport)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Richtige Reihenfolge der Verlinkungen
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::PrintSupport co2_backend qcustomplot)


set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()




