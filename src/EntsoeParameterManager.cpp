#include "EntsoeParameterManager.h"

EntsoeParameterManager::EntsoeParameterManager(){
    psrTypes = {
        {"A03", "Mixed"},
        {"A04", "Generation"},
        {"A05", "Load"},
        {"BO1", "Biomass"},
        {"BO2", "Fossil Brown coal"},
        {"BO3", "Fossil Coal-derived gas"},
        {"BO4", "Fossil Gas"},
        {"BO5", "Fossil Hard coal"},
        {"BO6", "Fossil Oil"},
        {"BO7", "Fossil Oil shale"},
        {"BO8", "Fossil Peat"},
        {"BO9", "Geothermal"},
        {"B10", "Hydro Pumped Storage"},
        {"B11", "Hydro Run-of-river and poundage"},
        {"B12", "Hydro Water Reservoir"},
        {"B13", "Marine"},
        {"B14", "Nuclear"},
        {"B15", "Other renewable"},
        {"B16", "Solar"},
        {"B17", "Waste"},
        {"B18", "Wind Offshore"},
        {"B19", "Wind Onshore"},
        {"B20", "Other"},
        {"B25", "Energy storage"},
    };

    documentTypes = {
        {"A01", "Day ahead"},
        {"A02", "Intra day incremental"},
        {"A16", "Realised"},
        {"A18", "Intraday total"},
        {"A31", "Week ahead"},
        {"A32", "Month ahead"},
        {"A33", "Year ahead"},
        {"A39", "Synchronisation process"},
        {"A40", "Intraday process"},
        {"A46", "Replacement reserve"},
        {"A47", "Manual frequency restoration reserve"},
        {"A51", "Automatic frequency restoration reserve"},
        {"A52", "Frequency containment reserve"},
        {"A56", "Frequency restoration reserve"},
        {"A60", "Scheduled activation mFRR"},
        {"A61", "Direct activation mFRR"},
        {"A67", "Central Selection aFRR"},
        {"A68", "Local Selection aFRR"}
    };

    processTypes = {
        {"A01", "Day ahead"},
        {"A02", "Intra day incremental"},
        {"A09", "Finalized schedule"},
        {"A11", "Aggregated energy data report"},
        {"A15", "Acquiring system operator reserve schedule"},
        {"A16", "Realised"},
        {"A18", "Intraday total"},
        {"A24", "Bid document"},
        {"A25", "Allocation result document"},
        {"A26", "Capacity document"},
        {"A31", "Agreed capacity"},
        {"A32", "Month ahead"},
        {"A33", "Year ahead"},
        {"A37", "Reserve bid document"},
        {"A38", "Reserve allocation result document"},
        {"A39", "Synchronisation process"},
        {"A40", "Intraday process"},
        {"A44", "Price Document"},
        {"A46", "Replacement reserve"},
        {"A47", "Manual frequency restoration reserve"},
        {"A51", "Automatic frequency restoration reserve"},
        {"A52", "Frequency containment reserve"},
        {"A56", "Frequency restoration reserve"},
        {"A60", "Scheduled activation mFRR"},
        {"A61", "Estimated Net Transfer Capacity"},
        {"A63", "Redispatch notice"},
        {"A65", "System total load"},
        {"A67", "Central Selection aFRR"},
        {"A68", "Installed generation per type"},
        {"A69", "Wind and solar forecast"},
        {"A70", "Load forecast margin"},
        {"A71", "Generation forecast"},
        {"A72", "Reservoir filling information"},
        {"A73", "Actual generation"},
        {"A74", "Wind and solar generation"},
        {"A75", "Actual generation per type"},
        {"A76", "Load unavailability"},
        {"A77", "Production unavailability"},
        {"A78", "Transmission unavailability"},
        {"A79", "Offshore grid infrastructure unavailability"},
        {"A80", "Generation unavailability"},
        {"A81", "Contracted reserves"},
        {"A82", "Accepted offers"},
        {"A83", "Activated balancing quantities"},
        {"A84", "Activated balancing prices"},
        {"A85", "Imbalance prices"},
        {"A86", "Imbalance volume"},
        {"A87", "Financial situation"},
        {"A88", "Cross border balancing"},
        {"A89", "Contracted reserve prices"},
        {"A90", "Interconnection network expansion"},
        {"A91", "Counter trade notice"},
        {"A92", "Congestion costs"},
        {"A93", "DC link capacity"},
        {"A94", "Non EU allocations"},
        {"A95", "Configuration document"},
        {"B11", "Flow-based allocations"},
        {"B17", "Aggregated netted external TSO schedule document"},
        {"B45", "Bid Availability Document"}
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