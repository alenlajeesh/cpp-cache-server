#include "../include/persistence.h"
#include "../include/config.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
extern std::unordered_map<std::string, std::string> store;
std::string DB_FILE()
{
    return config.data_dir + "/dump.rdb";
}
void save_db()
{
    std::ofstream file(DB_FILE());
    if (!file.is_open())
    {
        std::cerr << "Failed to open DB file for writing\n";
        return;
    }
    for (auto &pair : store)
    {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
}
void load_db()
{
    std::ifstream file(DB_FILE());
    if (!file.is_open())
        return;
    std::string key, value;
    while (file >> key >> value)
    {
        store[key] = value;
    }
    file.close();
}
