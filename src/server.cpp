#include "../include/server.h"
#include "../include/database.h"
#include "../include/command_parser.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void Server::start(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server listening on port " << port << std::endl;

    Database db;
    CommandParser parser;

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);

        char buffer[1024] = {0};
        read(client, buffer, 1024);

        std::string input(buffer);
        auto tokens = parser.parse(input);

        std::string response;

        if (tokens[0] == "SET" && tokens.size() == 3) {
            db.set(tokens[1], tokens[2]);
            response = "OK\n";
        }
        else if (tokens[0] == "GET" && tokens.size() == 2) {
            response = db.get(tokens[1]) + "\n";
        }
        else if (tokens[0] == "DEL" && tokens.size() == 2) {
            response = db.del(tokens[1]) ? "1\n" : "0\n";
        }
        else {
            response = "ERROR\n";
        }

        send(client, response.c_str(), response.size(), 0);

        close(client);
    }
}
