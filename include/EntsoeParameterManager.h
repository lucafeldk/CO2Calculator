#ifndef ENTSOE_PARAMETER_MANAGER_H
#define ENTSOE_PARAMETER_MANAGER_H

#include <string>
#include <unordered_map>

class EntsoeParameterManager{
    private:
        std::unordered_map<std::string, std::string> psrTypes;
        std::unordered_map<std::string, std::string> documentTypes;
        std::unordered_map<std::string, std::string> processTypes;
        std::unordered_map<std::string, std::string> inDomains;

    public:
        EntsoeParameterManager(); // Constructor

        std::string getPsrType(const std::string& code) const;
        std::string getDocumentType(const std::string& code) const;
        std::string getProcessType(const std::string& code) const;
        std::string getInDomain(const std::string& code) const;
};

#endif