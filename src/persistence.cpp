#include "../include/persistence.h"
#include "../include/database.h"

#include <fstream>
const std::string DB_FILE = "data/dump.rdb";
void save_db()
{
    std::ofstream file(DB_FILE);
    for (auto &pair : get_store())
    {
        file << pair.first << " " << pair.second << "\n";
    }
}

void load_db()
{
    std::ifstream file(DB_FILE);
    std::string key, value;
    while (file >> key >> value)
    {
        set_key(key, value);
    }
}
