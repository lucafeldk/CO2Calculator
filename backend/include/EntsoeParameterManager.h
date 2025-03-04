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
        
        // Getter methods Key -> Value 
        std::string getPsrType(const std::string& name) const;
        std::string getDocumentType(const std::string& name) const;
        std::string getProcessType(const std::string& name) const;
        std::string getInDomain(const std::string& name) const;

        // Getter methods Value -> Key 
        std::string getPsrTypeKey(const std::string& value) const;
        std::string getDocumentTypeKey(const std::string& value) const;
        std::string getProcessTypeKey(const std::string& value) const;
        std::string getInDomainKey(const std::string& value) const;
};

#endif