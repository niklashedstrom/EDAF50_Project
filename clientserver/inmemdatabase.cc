#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include "newsgroup.h"
#include "inmemdatabase.h"
using namespace std;

void InMemDatabase::addNewsGroup(const string& name) {
    NewsGroup newGroup;
    newGroup.id = newsgroupCounter++;
    newGroup.name = name;

    newsGroups.emplace(newGroup.id, newGroup);
}

void InMemDatabase::removeNewsGroup(long newsGroupId) {
    newsGroups.erase(newsGroupId);
}

void InMemDatabase::addArticle(long newsGroupId, Article& article) {
    article.id = articleCounter++;
    newsGroups[newsGroupId].articles.emplace(article.id, article);
}

void InMemDatabase::removeArticle(long newsGroupId, long articleId) {
    newsGroups[newsGroupId].articles.erase(articleId);
}

std::list<Article> InMemDatabase::getArticles(long newsGroupId) {
    auto articles = newsGroups[newsGroupId].articles;
    list<Article> tmpList;
    for(auto it = articles.begin(); it != articles.end(); ++it){
        auto val = (*it).second;
        tmpList.emplace_back(val);
    }
    tmpList.sort();
    return tmpList;
}

std::list<NewsGroup> InMemDatabase::getNewsGroups() {
    list<NewsGroup> tmpList;
    for(auto it = newsGroups.begin(); it != newsGroups.end(); ++it){
        auto val = (*it).second;
        tmpList.emplace_back(val);
    }
    tmpList.sort();
    return tmpList;
}
