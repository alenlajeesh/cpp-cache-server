#include "../include/server.h"
#include "../include/persistence.h"

int main()
{
    load_db();

    Server server;
    server.start(6379);

    return 0;
}
