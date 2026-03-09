#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>
#include <chrono>

class Database {
public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);

    bool expire(const std::string& key, int seconds);
    int ttl(const std::string& key);

private:
    std::unordered_map<std::string, std::string> store;

    std::unordered_map<std::string,
        std::chrono::steady_clock::time_point> expirations;

    void check_expired(const std::string& key);
};

#endif
