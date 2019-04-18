#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include <map>
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
    article.id = newsGroups[newsGroupId].articleId;
    newsGroups[newsGroupId].articles.emplace(newsGroups[newsGroupId].articleId, article);
    newsGroups[newsGroupId].articleId++;
}

void InMemDatabase::removeArticle(long newsGroupId, long articleId) {
    newsGroups[newsGroupId].articles.erase(articleId);
}

list<Article> InMemDatabase::getArticles(long newsGroupId) {
    auto articles = newsGroups[newsGroupId].articles;
    list<Article> tmpList;
    if(!articles.empty()){
        for(auto it = articles.begin(); it != articles.end(); ++it){
            auto val = (*it).second;
            tmpList.emplace_back(val);
        }
        tmpList.sort();
    }
    return tmpList;
}

list<NewsGroup> InMemDatabase::getNewsGroups() {
    list<NewsGroup> tmpList;
    if(!newsGroups.empty()){
        for(auto it = newsGroups.begin(); it != newsGroups.end(); ++it){
            auto val = (*it).second;
            tmpList.emplace_back(val);
        }
        tmpList.sort();
    }
    return tmpList;
}
