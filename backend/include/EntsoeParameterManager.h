#ifndef ENTSOE_PARAMETER_MANAGER_H
#define ENTSOE_PARAMETER_MANAGER_H

#include <string>
#include <unordered_map>

class EntsoeParameterManager{
    private:

        //Origial Maps (Key -> Value)
        std::unordered_map<std::string, std::string> psrTypes;
        std::unordered_map<std::string, std::string> documentTypes;
        std::unordered_map<std::string, std::string> processTypes;
        std::unordered_map<std::string, std::string> inDomains;

        // Reverse Maps (Value -> Key)
        std::unordered_map<std::string, std::string> psrTypesReverse;
        std::unordered_map<std::string, std::string> processTypesReverse;
        std::unordered_map<std::string, std::string> documentTypesReverse;
        std::unordered_map<std::string, std::string> inDomainsReverse;

    public:
        EntsoeParameterManager(); // Constructor
        
        // Getter methods Key -> Value 
        std::string getPsrType(const std::string& key) const;
        std::string getDocumentType(const std::string& key) const;
        std::string getProcessType(const std::string& key) const;
        std::string getInDomain(const std::string& key) const;

        // Getter methods Value -> Key 
        std::string getPsrTypeKey(const std::string& value) const;
        std::string getDocumentTypeKey(const std::string& value) const;
        std::string getProcessTypeKey(const std::string& value) const;
        std::string getInDomainKey(const std::string& value) const;
};

#endif