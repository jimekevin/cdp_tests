#include "Config.h"

#include <fstream>

bool Config::parseSimple(const std::string& path) {
    std::ifstream conf(path);

    if (!conf.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(conf, line)) {
        std::cout << line << "\n\n";
    }

    conf.close();

    return true;
}
