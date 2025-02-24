#ifndef CO2_CALCULATOR_H
#define CO2_CALCULATOR_H

#include <string>
#include <unordered_map>

class CO2Calculator{
    private:
        std::unordered_map<std::string, double> emissionFactor;
    public:
        CO2Calculator();
        double calcCO2(double power, std::string& psrType);
};

#endif