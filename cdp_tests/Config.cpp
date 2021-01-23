#include "Config.h"

#include <fstream>

bool Config::parseSimple(const std::string& path) {
    std::ifstream conf(path);

    if (!conf.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(conf, line)) {
        auto pos = line.find(':');
        auto key = line.substr(0, pos);
        auto value = line.substr(pos + 2);

        if (value.length() ==  0) {
            continue;
        }

        data[key] = value;

        std::cout << "Config: " << key << " -> " << value << "\n";
    }

    conf.close();

    return true;
}


std::string Config::getValue(const std::string& key) {
    if (data.find(key) != data.end()) {
        return data[key];
    }
    return "";
}

int Config::getValueI(const std::string& key) {
    return std::stoi(getValue(key));
}

float Config::getValueF(const std::string& key) {
    return std::stof(getValue(key));
}
