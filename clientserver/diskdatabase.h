#ifndef DISKDATABASE_H
#define DISKDATABASE_H

#include "database.h"
#include <sqlite3.h>
#include <vector>
#include <string>

using Helper = std::vector<std::string>;
using Helpers = std::vector<Helper>;

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
    bool exists(const char *filename);
    int exit = 0;
    Helpers select_stmt(const char* stmt);
    void sql_stmt(const char* stmt);
};

#endif
