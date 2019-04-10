#ifndef YOURSEVER_H
#define YOURSEVER_H

#include <iostream>
#include "server.h"
#include "database.h"

class YourServer{
public:
    YourServer(const Server& s, const Database& d): server(s), db(d) {}
    int run();

private:
    Server server;
    Database db;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Wrong number of arguments, 1 for in mem db, 2 for disk db" << std::endl;
        return 1;
    }
    Database db;
    if (argv[1] == "1") {
        db = InMemDatabase();
    } else if (argv[1] == "2") {
        // Create new disk database
    } else {
        std::cout << "Wrong argument, 1 for in mem db, 2 for disk db" << std::endl;
        return 2;
    }

    Server server(8888);

    YourServer yourServer(server, db);
    return yourServer.run();
}

#endif