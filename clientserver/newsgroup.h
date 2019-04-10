#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <map>
#include <string>

struct Article {
    long id;
    std::string title;
    std::string author;
    std::string content;
};

struct NewsGroup {
    long id;
    std::string name;
    std::map<long, Article> articles;
};

#endif