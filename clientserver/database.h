#ifndef DATABASE_H
#define DATABASE_H

#include <list>
#include <string>
#include "newsgroup.h"

class Database {
public:
    virtual void addNewsGroup(const std::string&){}
    virtual void removeNewsGroup(long newsGroupId){}
    virtual void addArticle(long newsGroupId, Article&){}
    virtual void removeArticle(long newsGroupId, long articleId){}
    virtual std::list<Article> getArticles(long newsGroupId){}
    virtual std::list<NewsGroup> getNewsGroups(){}
};

#endif