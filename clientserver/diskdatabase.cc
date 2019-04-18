#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include "newsgroup.h"
#include "inmemdatabase.h"
using namespace std;


DiskDatabase::DiskDatabase(){}

void DiskDatabase::addNewsGroup(const string& name) {}

void DiskDatabase::removeNewsGroup(long newsGroupId) {}

void DiskDatabase::addArticle(long newsGroupId, Article& article) {}

void DiskDatabase::removeArticle(long newsGroupId, long articleId) {}

list<Article> DiskDatabase::getArticles(long newsGroupId) {}

list<NewsGroup> DiskDatabase::getNewsGroups() {}
