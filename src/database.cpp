#include "../include/database.h"
#include "../include/persistence.h"
#include <unordered_map>
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>
std::unordered_map<std::string, std::string> store;
std::unordered_map<std::string, std::chrono::steady_clock::time_point> expirations;
bool is_expired(const std::string &key)
{
    if (expirations.find(key) == expirations.end())
        return false;
    auto now = std::chrono::steady_clock::now();
    if (now >= expirations[key])
    {
        store.erase(key);
        expirations.erase(key);
        return true;
    }
    return false;
}
void cleanup_expired_keys()
{
    auto now = std::chrono::steady_clock::now();
    for (auto it = expirations.begin(); it != expirations.end();)
    {
        if (now >= it->second)
        {
            store.erase(it->first);
            it = expirations.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
std::string handle_command(const std::vector<std::string> &args)
{
    if (args.empty())
        return "ERROR\n";

    std::string cmd = args[0];

	if (cmd == "INFO")
	{
		std::stringstream info;
		info << "cache Server\n";
		info << "Keys: " << store.size() << "\n";
		info << "Version: 0.9.0\n";
		return info.str();
	}

	if (cmd == "SAVE")
	{
		save_db();
		return "OK\n";
	}
	if (cmd == "QUIT")
	{
		return "BYE\n";
	}

    if (cmd == "PING")
        return "PONG\n";

    if (cmd == "SET")
    {
        if (args.size() != 3)
            return "ERROR\n";

        store[args[1]] = args[2];
        expirations.erase(args[1]);

        save_db();
        return "OK\n";
    }
    if (cmd == "GET")
    {
        if (args.size() != 2)
            return "ERROR\n";
        std::string key = args[1];

        if (store.find(key) == store.end())
            return "(nil)\n";

        if (is_expired(key))
            return "(nil)\n";
        return store[key] + "\n";
    }
    if (cmd == "DEL")
    {
        if (args.size() != 2)
            return "ERROR\n";
        int removed = store.erase(args[1]);
        expirations.erase(args[1]);
        save_db();
        return std::to_string(removed) + "\n";
    }
    if (cmd == "EXPIRE")
    {
        if (args.size() != 3)
            return "ERROR\n";
        std::string key = args[1];
        int seconds = std::stoi(args[2]);
        if (store.find(key) == store.end())
            return "0\n";
        expirations[key] =
            std::chrono::steady_clock::now() +
            std::chrono::seconds(seconds);

        return "1\n";
    }

    if (cmd == "TTL")
    {
        if (args.size() != 2)
            return "ERROR\n";
        std::string key = args[1];
        if (store.find(key) == store.end())
            return "-2\n";
        if (expirations.find(key) == expirations.end())
            return "-1\n";
        auto now = std::chrono::steady_clock::now();
        auto remaining =
            std::chrono::duration_cast<std::chrono::seconds>(
                expirations[key] - now)
                .count();
        if (remaining <= 0)
        {
            store.erase(key);
            expirations.erase(key);
            return "-2\n";
        }
        return std::to_string(remaining) + "\n";
    }
    return "ERROR\n";
}
