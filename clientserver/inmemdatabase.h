#ifndef INMEMDATABASE_H
#define INMEMDATABASE_H

#include "database.h"

class InMemDatabase : public Database {
public:
    InMemDatabase() = default;
    void addNewsGroup(const std::string&);
    void removeNewsGroup(long newsGroupId);
    void addArticle(long newsGroupId, const Article&);
    void removeArticle(long newsGroupId, long articleId);
    std::list<Article> getArticles(long newsGroupId);
    std::list<NewsGroup> getNewsGroups();

private:
    std::map<long, NewsGroup> newsGroups;
    long counter = 1;
};

#endif