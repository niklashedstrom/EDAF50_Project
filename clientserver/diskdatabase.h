#ifndef DISKDATABASE_H
#define DISKDATABASE_H

#include "database.h"

class DiskDatabase : public Database {
public:
    DiskDatabase();
    void addNewsGroup(const std::string&);
    void removeNewsGroup(long newsGroupId);
    void addArticle(long newsGroupId, Article&);
    void removeArticle(long newsGroupId, long articleId);
    std::list<Article> getArticles(long newsGroupId);
    std::list<NewsGroup> getNewsGroups();

private:
    long newsgroupCounter = 1;
};

#endif