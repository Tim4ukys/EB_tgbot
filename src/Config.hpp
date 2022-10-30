#pragma once

#include <string>
#include <fstream>
#include <iostream>

// json-modern
#include "../third_party/json-modern/json.hpp"
using json = nlohmann::json;

class Config {
    std::string m_fileName;

protected:
    json m_json;

public:
    friend class CoordsData;

    /**
     * @brief Construct a new Config object
     * 
     * @param fileName Имя файла
     */
    Config(const std::string &fileName) : m_fileName(fileName) {
        std::ifstream file{m_fileName};
        if (file.is_open()) {
            file >> m_json;
        }
    }
    ~Config() {
        std::ofstream file{m_fileName};
        file << m_json.dump(4);
        // std::cout << m_json.dump(4);
        file.close();
    }
};