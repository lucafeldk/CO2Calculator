#include "APIClient.h"
#include <cpr/cpr.h>

void APIClient::get_request(std::string& param, const std::string& api_key){
    paramURL = param + "&securityToken=" + api_key;
    cpr::Response response = cpr::Get(cpr::Url{baseURL + paramURL});
    if (response.error) {
        throw std::runtime_error("Network error: " + response.error.message);
    }

    if (response.status_code != 200) {
        throw std::runtime_error("HTTP error: Status code " + std::to_string(response.status_code) +
                                 " - " + response.text);
    }
        std::cout << "Status Code" << response.status_code << std::endl;
        std::cout << "Response:" << response.text << std::endl;
}