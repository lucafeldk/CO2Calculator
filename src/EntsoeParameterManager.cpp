#include "EntsoeParameterManager.h"

EntsoeParameterManager::EntsoeParameterManager(){
    psrTypes = {
        {"Mixed", "A03"},
        {"Generation", "A04"},
        {"Load", "A05"},
        {"Biomass", "B01"},
        {"Fossil Brown coal", "B02"},
        {"Fossil Coal-derived gas", "B03"},
        {"Fossil Gas", "B04"},
        {"Fossil Hard coal", "B05"},
        {"Fossil Oil", "B06"},
        {"Fossil Oil shale", "B07"},
        {"Fossil Peat", "B08"},
        {"Geothermal", "B09"},
        {"Hydro Pumped Storage", "B10"},
        {"Hydro Run-of-river and poundage", "B11"},
        {"Hydro Water Reservoir", "B12"},
        {"Marine", "B13"},
        {"Nuclear", "B14"},
        {"Other renewable", "B15"},
        {"Solar", "B16"},
        {"Waste", "B17"},
        {"Wind Offshore", "B18"},
        {"Wind Onshore", "B19"},
        {"Other", "B20"},
        {"Energy storage", "B25"},
    };

    processTypes = {
        {"Day ahead", "A01"},
        {"Intra day incremental", "A02"},
        {"Realised", "A16"},
        {"Intraday total", "A18"},
        {"Week ahead", "A31"},
        {"Month ahead", "A32"},
        {"Year ahead", "A33"},
        {"Synchronisation process", "A39"},
        {"Intraday process", "A40"},
        {"Replacement reserve", "A46"},
        {"Manual frequency restoration reserve", "A47"},
        {"Automatic frequency restoration reserve", "A51"},
        {"Frequency containment reserve", "A52"},
        {"Frequency restoration reserve", "A56"},
        {"Scheduled activation mFRR", "A60"},
        {"Direct activation mFRR", "A61"},
        {"Central Selection aFRR", "A67"},
        {"Local Selection aFRR", "A68"}
    };

    documentTypes = {
        {"Day ahead", "A01"},
        {"Intra day incremental", "A02"},
        {"Finalized schedule", "A09"},
        {"Aggregated energy data report", "A11"},
        {"Acquiring system operator reserve schedule", "A15"},
        {"Realised", "A16"},
        {"Intraday total", "A18"},
        {"Bid document", "A24"},
        {"Allocation result document", "A25"},
        {"Capacity document", "A26"},
        {"Agreed capacity", "A31"},
        {"Month ahead", "A32"},
        {"Year ahead", "A33"},
        {"Reserve bid document", "A37"},
        {"Reserve allocation result document", "A38"},
        {"Synchronisation process", "A39"},
        {"Intraday process", "A40"},
        {"Price Document", "A44"},
        {"Replacement reserve", "A46"},
        {"Manual frequency restoration reserve", "A47"},
        {"Automatic frequency restoration reserve", "A51"},
        {"Frequency containment reserve", "A52"},
        {"Frequency restoration reserve", "A56"},
        {"Scheduled activation mFRR", "A60"},
        {"Estimated Net Transfer Capacity", "A61"},
        {"Redispatch notice", "A63"},
        {"System total load", "A65"},
        {"Central Selection aFRR", "A67"},
        {"Installed generation per type", "A68"},
        {"Wind and solar forecast", "A69"},
        {"Load forecast margin", "A70"},
        {"Generation forecast", "A71"},
        {"Reservoir filling information", "A72"},
        {"Actual generation", "A73"},
        {"Wind and solar generation", "A74"},
        {"Actual generation per type", "A75"},
        {"Load unavailability", "A76"},
        {"Production unavailability", "A77"},
        {"Transmission unavailability", "A78"},
        {"Offshore grid infrastructure unavailability", "A79"},
        {"Generation unavailability", "A80"},
        {"Contracted reserves", "A81"},
        {"Accepted offers", "A82"},
        {"Activated balancing quantities", "A83"},
        {"Activated balancing prices", "A84"},
        {"Imbalance prices", "A85"},
        {"Imbalance volume", "A86"},
        {"Financial situation", "A87"},
        {"Cross border balancing", "A88"},
        {"Contracted reserve prices", "A89"},
        {"Interconnection network expansion", "A90"},
        {"Counter trade notice", "A91"},
        {"Congestion costs", "A92"},
        {"DC link capacity", "A93"},
        {"Non EU allocations", "A94"},
        {"Configuration document", "A95"},
        {"Flow-based allocations", "B11"},
        {"Aggregated netted external TSO schedule document", "B17"},
        {"Bid Availability Document", "B45"}
    };

    inDomains = {
        {"Germany", "10Y1001A1001A83F"}
    };
}

std::string EntsoeParameterManager::getPsrType(const std::string& code) const {
    return psrTypes.count(code) ? psrTypes.at(code) : "Unknown";
}

std::string EntsoeParameterManager::getDocumentType(const std::string& code) const {
    return documentTypes.count(code) ? documentTypes.at(code) : "Unknown";
}

std::string EntsoeParameterManager::getProcessType(const std::string& code) const {
    return processTypes.count(code) ? processTypes.at(code) : "Unknown";
}

std::string EntsoeParameterManager::getInDomain(const std::string& country) const {
    return inDomains.count(country) ? inDomains.at(country) : "Unknown";
}