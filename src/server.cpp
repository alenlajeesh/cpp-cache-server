#include "../include/server.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define BUFFER_SIZE 4096
std::unordered_map<std::string, std::string> store;
std::unordered_map<std::string, std::chrono::steady_clock::time_point> expirations;
const std::string DB_FILE = "dump.rdb";
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
void save_db()
{
    std::ofstream file(DB_FILE);
    for (auto &pair : store)
    {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
}
void load_db()
{
    std::ifstream file(DB_FILE);
    if (!file.is_open())
        return;
    std::string key, value;
    while (file >> key >> value)
    {
        store[key] = value;
    }
    file.close();
}
std::string handle_command(const std::string &command)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    if (cmd == "SET")
    {
        std::string key, value;
        iss >> key >> value;
        store[key] = value;
        expirations.erase(key);
        save_db();
        return "OK\n";
    }
    else if (cmd == "GET")
    {
        std::string key;
        iss >> key;
        if (store.find(key) == store.end())
            return "(nil)\n";
        if (is_expired(key))
            return "(nil)\n";
        return store[key] + "\n";
    }
    else if (cmd == "DEL")
    {
        std::string key;
        iss >> key;
        int removed = store.erase(key);
        expirations.erase(key);
        save_db();
        return std::to_string(removed) + "\n";
    }
    else if (cmd == "EXPIRE")
    {
        std::string key;
        int seconds;
        iss >> key >> seconds;
        if (store.find(key) == store.end())
            return "0\n";
        expirations[key] =
            std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
        return "1\n";
    }
    else if (cmd == "TTL")
    {
        std::string key;
        iss >> key;
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
void Server::start(int port)
{
    load_db();
    std::cout << "Database loaded\n";
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(server_fd, (sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "MiniRedis running on port " << port << std::endl;
    while (true)
    {
        int client =
            accept(server_fd, nullptr, nullptr);
        if (client < 0)
        {
            perror("accept failed");
            continue;
        }
        char buffer[BUFFER_SIZE];
        while (true)
        {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes = recv(client, buffer, BUFFER_SIZE, 0);
            if (bytes <= 0)
                break;
            std::stringstream input(buffer);
            std::string line;
            while (std::getline(input, line))
            {
                if (line.empty())
                    continue;
                std::string response = handle_command(line);
                send(client, response.c_str(), response.size(), 0);
            }
        }
        close(client);
    }
}
