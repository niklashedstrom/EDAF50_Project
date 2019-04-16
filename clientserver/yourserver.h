#ifndef YOURSEVER_H
#define YOURSEVER_H

#include <iostream>
#include "server.h"
#include "database.h"
#include "inmemdatabase.h"

class YourServer{
public:
    YourServer(const Database& d): db(d) {}

private:
    Database db;
};

#endif