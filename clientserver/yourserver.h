#ifndef YOURSEVER_H
#define YOURSEVER_H

#include <iostream>
#include "server.h"
#include "database.h"
#include "inmemdatabase.h"

class YourServer{
public:
    YourServer(const Database& d): db(d) {}
    Server init(int argc, char* argv[]);
//    int run();

private:
    //Server server;
    Database db;
};

#endif