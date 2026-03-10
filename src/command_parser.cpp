#include "../include/command_parser.h"
#include "../include/database.h"
#include "../include/persistence.h"

#include <sstream>

std::string parse_command(const std::string &command)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    if (cmd == "SET")
    {
        std::string key, value;
        iss >> key >> value;
        set_key(key, value);
        save_db();
        return "OK\n";
    }
    if (cmd == "GET")
    {
        std::string key;
        iss >> key;
        std::string value = get_key(key);
        if (value.empty())
            return "(nil)\n";
        return value + "\n";
    }
    if (cmd == "DEL")
    {
        std::string key;
        iss >> key;
        int removed = del_key(key);
        save_db();
        return std::to_string(removed) + "\n";
    }
    if (cmd == "EXPIRE")
    {
        std::string key;
        int seconds;
        iss >> key >> seconds;
        set_expire(key, seconds);
        return "1\n";
    }
    if (cmd == "TTL")
    {
        std::string key;
        iss >> key;
        return std::to_string(ttl(key)) + "\n";
    }
    return "ERROR\n";
}
