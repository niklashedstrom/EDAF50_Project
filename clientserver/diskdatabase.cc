#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include "diskdatabase.h"

using namespace std;

using Helper = vector<string>;
using Helpers = vector<Helper>;

sqlite3* DB;

DiskDatabase::DiskDatabase() {
    exit = 0;
    char* messageError;

    ifstream ifile("diskdatabase.db");
    if(!ifile){
        cout << "No .db file, creating a new!" << endl;
        exit = sqlite3_open("diskdatabase.db", &DB);

        /** Creates outline for database */
        string sql =
                 "CREATE TABLE newsgroups("
                 "ng_id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "ng_name   TEXT UNIQUE NOT NULL);"
                 "CREATE TABLE articles("
                 "at_id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "at_title  TEXT NOT NULL,"
                 "at_text   TEXT,"
                 "au_name   TEXT,"
                 "ng_id     INTEGER,"
                 "FOREIGN KEY (ng_id) REFERENCES newsgroups(ng_id));";
    
        
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    } else {
        cout << "Opening existing .db file!" << endl;
        exit = sqlite3_open("diskdatabase.db", &DB);
    }
    

    if (exit != SQLITE_OK) {
      cerr << "One or more errors occured:" << endl;
      cout << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Success" << endl;
    }
}


/** Borde funka */
void DiskDatabase::addNewsGroup(const string& name) {
    exit = 0;

    string sql = "INSERT INTO newsgroups (ng_name) VALUES ('" + name + "');";
    char* messageError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

/** Borde funka */
void DiskDatabase::removeNewsGroup(long newsGroupId) {
    exit = 0;

    string sql = "DELETE FROM newsgroups WHERE ng_id = " + to_string(newsGroupId) + ";";
    char* messageError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

void DiskDatabase::addArticle(long newsGroupId, Article& article) {
    exit = 0;
    string title = article.title;
    string text = article.content;
    string name = article.author;

    string sql = "INSERT INTO articles (at_title, at_text, au_name, ng_id)"
                "VALUES ('" + title + "', '" + text + "', '" + name + "', '" + to_string(newsGroupId) + "');";

    char* messageError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

void DiskDatabase::removeArticle(long newsGroupId, long articleId) {
    exit = 0;

    string sql = "DELETE FROM articles WHERE "
               "at_id = " + to_string(articleId) + " AND ng_id = " + to_string(newsGroupId) + ";";

    char* messageError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

list<Article> DiskDatabase::getArticles(long newsGroupId) {
    list<Article> tmp;
    string sql = "SELECT * FROM articles WHERE ng_id = " + to_string(newsGroupId) + ";";
    Helpers helpers = select_stmt(sql.c_str());
    for(auto& h : helpers){
        Article art;
        art.id = stoi(h[0]);
        art.title = h[1];
        art.content = h[2];
        art.author = h[3];
        tmp.push_back(art);
    }
    return tmp;
}

list<NewsGroup> DiskDatabase::getNewsGroups() {
    list<NewsGroup> tmp;
    Helpers helpers = select_stmt("SELECT * FROM newsgroups;");
    for(auto& h : helpers){
        NewsGroup ng;
        ng.id = stoi(h[0]);
        ng.name = h[1];
        tmp.push_back(ng);
    }

    return tmp;
}
 
// Help functions for sqlite3

Helpers DiskDatabase::select_stmt(const char* stmt){
    Helpers helpers;
    char* errmsg;
    int ret = sqlite3_exec(DB, stmt, [](void* used, int argc, char **argv, char **azColName) 
                                {Helpers* helpers = static_cast<Helpers*>(used);
                                 try {
                                     helpers->emplace_back(argv, argv + argc);
                                 } catch (...){
                                     return 1;
                                 }
                                 return 0;}, &helpers, &errmsg);
    if(ret != SQLITE_OK){
        cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
    } else {
        std::cerr << helpers.size() << " records returned.\n";
    }
    return helpers;
}

void DiskDatabase::sql_stmt(const char* stmt){
    char* errmsg;
    int ret = sqlite3_exec(DB, stmt, 0, 0, &errmsg);
    if(ret != SQLITE_OK){
        cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
    }
}




/*
list<string> Database::getArticles() const {
  ostringstream oss;
  list<string> grps;
  sqlite3_stmt *stmt = 0;
  int rc = 0;

  string sql = "SELECT at_title, at_text, au_name"
               "FROM   articles"
               "JOIN   posted_articles"
               "USING  (at_id)"
               "WHERE  posted_articles.ng_id = 1;";

  string sql = "SELECT * FROM articles;";

  rc = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL);

  while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    for(int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
      string col(reinterpret_cast<const char*>(sqlite3_column_text(stmt, colIndex)));

      if(colIndex + 1 == sqlite3_column_count(stmt)) {
        oss << col;
      } else {
        oss << col << " ";
      }
    }

    grps.push_back(oss.str());
    oss = ostringstream();
  }

  rc = sqlite3_finalize(stmt);
  return grps;
}
*/

// list<NewsGroup> DiskDatabase::getNewsGroups() {}

