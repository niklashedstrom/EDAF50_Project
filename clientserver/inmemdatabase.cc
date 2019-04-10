#include <string>
#include <algorithm>
#include <list>
#include "newsgroup.h"
#include "inmemdatabase.h"
using namespace std;

void InMemDatabase::addNewsGroup(const string& name) {
    NewsGroup newGroup;
    newGroup.id = counter++;
    newGroup.name = name;

    newsGroups.insert(newGroup);
}

void InMemDatabase::removeNewsGroup(long newsGroupId) {
    newsGroups.erase(newsGroupId);
}

void InMemDatabase::addArticle(long newsGroupId, const Article& article) {
    newsGroups[newsGroupId].articles.insert(article);
}

void InMemDatabase::removeArticle(long newsGroupId, long articleId) {
    newsGroups[newsGroupId].articles.erase(articleId);
}

std::list<Article> InMemDatabase::getArticles(long newsGroupId) {
    auto articles = newsGroups[newsGroupId].articles;
    list<Article> tmpList;
    copy(articles.begin(), articles.end(), tmpList.begin());
    sort(tmpList.begin(), tmpList.end(), [] (const Article& a1, const Article& a2) {return a1.id < a2.id;});
    return tmpList;
}

std::list<NewsGroup> InMemDatabase::getNewsGroups() {
    list<NewsGroup> tmpList;
    copy(newsGroups.begin(), newsGroups.end(), tmpList.begin());
    sort(tmpList.begin(), tmpList.end(), [] (const NewsGroup& a1, const NewsGroup& a2) {return a1.id < a2.id;});
    return tmpList;
}
