#include "../include/server.h"
#include "../include/persistence.h"
#include "../include/config.h"
#include <iostream>
int main()
{	
	std::cout << "cache-server v1.0.0 starting...\n";
	load_config("cache-server.conf");
	std::cout << "Starting cache-server on port " << config.port << std::endl;
    load_db();
    Server server;
    server.start(config.port);
    return 0;
}
