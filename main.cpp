#include <cpr/cpr.h>
#include <iostream>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                           cpr::Header{{"User-Agent", "MyApp"}});
    std::cout << r.status_code << std::endl;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    std::cout << r.text;                         // JSON text string
    return 0;
}