#ifndef INMEMDATABASE_H
#define INMEMDATABASE_H

#include "database.h"

class InMemDatabase : Database {
public:
    Database() = default;

private:
    std::map<long, NewsGroup> newsGroups;
    long counter = 1;
};

#endif