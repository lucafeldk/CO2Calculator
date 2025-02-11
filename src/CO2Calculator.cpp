#include "CO2Calculator.h"

CO2Calculator::CO2Calculator(){
    emissionFactor ={       //EMISSION FACTOR IN gCO2eq/kWh
        {"B01", 266.8},     //"Biomass"
        {"BO2", 1209.9},    //"Fossil Brown coal"
        {"B03", 952},       //"Coal-derivded gas"
        {"B04", 467.4},     //"Fossil Gas"
        {"B05", 1051.5},    //"Fossil Hard coal"
        {"B06", 890.0},     //"Fossil Oil"
        {"B07", 0.0},       //"Fossil Oil shale"
        {"B08", 0.0},       //"Fossil Peat"
        {"B09", 182.6},     //"Geothermal"
        {"B10", 25.1},      //"Hydro Pumped Storage"
        {"B11", 2.7},       //"Hydro Run-of-river and poundage"
        {"B12", 25.1},      //"Hydro Water Reservoir"
        {"B13", 0.0},       //"Marine"
        {"B14", 32},        //"Nuclear"
        {"B15", 93},        //"Other renewable"
        {"B16", 66.7},      //"Solar"
        {"B17", 266.8},     //"Waste"
        {"B18", 10.5},      //"Wind Offshore"
        {"B19", 6},         //"Wind Onshore"
        {"B20", 683.8}      //"Other", 
    };
}

void CO2Calculator::calc_CO2(){
    
    emissionfactor
}