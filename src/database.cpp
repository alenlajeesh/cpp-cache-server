#include "../include/database.h"
#include <chrono>

void Database::set(const std::string& key, const std::string& value) {
    store[key] = value;
}

std::string Database::get(const std::string& key) {
    check_expired(key);

    if (store.find(key) != store.end()) {
        return store[key];
    }

    return "(nil)";
}

bool Database::del(const std::string& key) {
    expirations.erase(key);
    return store.erase(key) > 0;
}

bool Database::expire(const std::string& key, int seconds) {
    if (store.find(key) == store.end())
        return false;

    expirations[key] =
        std::chrono::steady_clock::now() +
        std::chrono::seconds(seconds);

    return true;
}

int Database::ttl(const std::string& key) {
    check_expired(key);

    if (expirations.find(key) == expirations.end())
        return -1;

    auto remaining =
        std::chrono::duration_cast<std::chrono::seconds>(
            expirations[key] - std::chrono::steady_clock::now());

    return remaining.count();
}

void Database::check_expired(const std::string& key) {
    if (expirations.find(key) == expirations.end())
        return;

    if (std::chrono::steady_clock::now() > expirations[key]) {
        store.erase(key);
        expirations.erase(key);
    }
}
