#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config
{
    int port = 6379;
    std::string data_dir = "data";
};

extern Config config;

void load_config(const std::string &filename);

#endif
