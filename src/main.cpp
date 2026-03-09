#include <iostream>
#include "../include/database.h"

int main() {
    Database db;

    db.set("name", "alen");

    std::cout << db.get("name") << std::endl;

    db.del("name");

    std::cout << db.get("name") << std::endl;

    return 0;
}
