#include "../include/database.h"

void Database::set(const std::string& key, const std::string& value) {
    store[key] = value;
}

std::string Database::get(const std::string& key) {
    if (store.find(key) != store.end()) {
        return store[key];
    }
    return "(nil)";
}

bool Database::del(const std::string& key) {
    return store.erase(key) > 0;
}
