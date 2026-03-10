#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <unordered_map>

bool key_exists(const std::string &key);

void set_key(const std::string &key, const std::string &value);

std::string get_key(const std::string &key);

int del_key(const std::string &key);

void set_expire(const std::string &key, int seconds);

long ttl(const std::string &key);

std::unordered_map<std::string, std::string>& get_store();

#endif
