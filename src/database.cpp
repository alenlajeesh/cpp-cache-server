#include "../include/database.h"
#include <unordered_map>
#include <chrono>
static std::unordered_map<std::string, std::string> store;
static std::unordered_map<std::string, std::chrono::steady_clock::time_point> expirations;
bool key_exists(const std::string &key)
{
    return store.find(key) != store.end();
}
void set_key(const std::string &key, const std::string &value)
{
    store[key] = value;
    expirations.erase(key);
}
std::string get_key(const std::string &key)
{
    if (!key_exists(key))
        return "";

    return store[key];
}
int del_key(const std::string &key)
{
    int removed = store.erase(key);
    expirations.erase(key);
    return removed;
}
void set_expire(const std::string &key, int seconds)
{
    expirations[key] =
        std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
}
long ttl(const std::string &key)
{
    if (expirations.find(key) == expirations.end())
        return -1;
    auto now = std::chrono::steady_clock::now();
    auto remaining =
        std::chrono::duration_cast<std::chrono::seconds>(
            expirations[key] - now)
            .count();
    return remaining;
}
std::unordered_map<std::string, std::string>& get_store()
{
    return store;
}
