#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <map>
#include <string>

struct Article {
    long id;
    std::string title;
    std::string author;
    std::string content;
    friend bool operator<(const Article& a1, const Article& a2){ return a1.id < a2.id;}
};

struct NewsGroup {
    long id;
    std::string name;
    std::map<long, Article> articles;
    long articleId = 1;
    friend bool operator<(const NewsGroup& n1, const NewsGroup& n2){ return n1.id < n2.id;}
};

#endif