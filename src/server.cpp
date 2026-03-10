#include "../include/server.h"
#include "../include/database.h"
#include "../include/command_parser.h"
#include "../include/persistence.h"
#include "../include/logger.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define BUFFER_SIZE 4096

void expiration_worker()
{
    while (true)
    {
        cleanup_expired_keys();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
void handle_client(int client)
{
	log_message("Client connected");
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
            auto args = parse_command(line);
			log_message(line);
            if (args.empty())
                continue;
            std::string response = handle_command(args);
            send(client, response.c_str(), response.size(), 0);
			if (args[0] == "QUIT"|| args[0] == "EXIT")
            {
                close(client);
                log_message("Client disconnected");
                return;
            }
        }
    }
	log_message("Client disconnected");
    close(client);
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
	std::thread(expiration_worker).detach();
    while (true)
    {
        int client = accept(server_fd, nullptr, nullptr);
        if (client < 0)
        {
            perror("accept failed");
            continue;
        }
        std::thread(handle_client, client).detach();
    }
}

