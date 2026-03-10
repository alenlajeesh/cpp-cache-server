#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

std::string handle_command(const std::vector<std::string> &args);

bool is_expired(const std::string &key);
void cleanup_expired_keys();

#endif
