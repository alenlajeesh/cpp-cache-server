#include "../include/server.h"
#include "../include/command_parser.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes <= 0)
            break;
        std::stringstream input(buffer);
        std::string line;
        while (std::getline(input, line))
        {
            if (line.empty())
                continue;
            std::string response = parse_command(line);
            send(client_fd, response.c_str(), response.size(), 0);
        }
    }
    close(client_fd);
}
void Server::start(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
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
    listen(server_fd, 10);
    std::cout << "cache-server running on port " << port << std::endl;
    while (true)
    {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0)
        {
            perror("accept failed");
            continue;
        }
        std::thread(handle_client, client_fd).detach();
    }
}
