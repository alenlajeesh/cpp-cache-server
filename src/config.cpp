#include "../include/config.h"
#include <fstream>
#include <sstream>
#include <iostream>
Config config;
void load_config(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "No config file found. Using defaults.\n";
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string key;
        std::string value;
        if (std::getline(ss, key, '=') && std::getline(ss, value))
        {
            if (key == "port")
                config.port = std::stoi(value);
            if (key == "data_dir")
                config.data_dir = value;
        }
    }
}
