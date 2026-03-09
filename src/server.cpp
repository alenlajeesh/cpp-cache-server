#include "../include/server.h"
#include "../include/database.h"
#include "../include/command_parser.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>

void Server::start(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        std::cerr << "Failed to create socket\n";
        return;
    }
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return;
    }
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed\n";
        return;
    }
    std::cout << "Server listening on port " << port << std::endl;
    Database db;
    CommandParser parser;
    while (true)
    {
        int client = accept(server_fd, nullptr, nullptr);
        if (client < 0) {
            std::cerr << "Client connection failed\n";
            continue;
        }
        while (true)
        {
            char buffer[1024] = {0};
            int bytes = read(client, buffer, sizeof(buffer));
            if (bytes <= 0)
                break;
            std::string input(buffer);
            std::stringstream ss(input);
            std::string line;
            while (std::getline(ss, line))
            {
                if (line.empty())
                    continue;
                auto tokens = parser.parse(line);
                if (tokens.empty())
                    continue;
                std::string command = tokens[0];
                std::transform(command.begin(), command.end(), command.begin(), ::toupper);
                std::string response;
                if (command == "SET" && tokens.size() == 3) {
                    db.set(tokens[1], tokens[2]);
                    response = "OK\n";
                }
                else if (command == "GET" && tokens.size() == 2) {
                    response = db.get(tokens[1]) + "\n";
                }
                else if (command == "DEL" && tokens.size() == 2) {
                    response = db.del(tokens[1]) ? "1\n" : "0\n";
                }
                else if (command == "EXPIRE" && tokens.size() == 3) {
                    int seconds = std::stoi(tokens[2]);
                    response = db.expire(tokens[1], seconds) ? "1\n" : "0\n";
                }
                else if (command == "TTL" && tokens.size() == 2) {
                    int ttl = db.ttl(tokens[1]);
                    response = std::to_string(ttl) + "\n";
                }
                else {
                    response = "ERROR\n";
                }
                send(client, response.c_str(), response.size(), 0);
            }
        }
        close(client);
    }
}
