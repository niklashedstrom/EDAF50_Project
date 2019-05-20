#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include "diskdatabase.h"

using namespace std;

sqlite3* DB;
int exit = 0;

DiskDatabase::DiskDatabase() {
  int exit = 0;

  if(exists("diskdatabase.db")) {
    exit = sqlite3_open("diskdatabase.db", &DB);
  } else {
    exit = sqlite3_open("diskdatabase.db", &DB);

    /** Creates outline for database */
    string sql = "CREATE TABLE newsgroups("
                 "ng_id     INT PRIMARY KEY AUTOINCREMENT,"
                 "ng_name   TEXT UNIQUE NOT NULL);"
                 "CREATE TABLE posted_articles("
                 "at_id     INTEGER NOT NULL,"
                 "ng_id     INTEGER NOT NULL,"
                 "FOREIGN KEY (at_id) REFERENCES articles(at_id),"
                 "FOREIGN KEY (ng_id) REFERENCES newsgroups(ng_id));"
                 "CREATE TABLE articles("
                 "at_id     INT PRIMARY KEY AUTOINCREMENT,"
                 "at_title  TEXT NOT NULL,"
                 "at_text   TEXT,"
                 "au_name   TEXT,"
                 "ng_id     INT,"
                 "FOREIGN KEY (ng_id) REFERENCES newsgroups(ng_id));";

    char* messageError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
      cerr << "One or more errors occured:" << endl;
      cout << messageError << endl;
      sqlite3_free(messageError);
    } else {
      cout << "Success" << endl;
    }
  }
}

/** Borde funka */
void DiskDatabase::addNewsGroup(const string& name) {
  int exit = 0;

  string sql = "INSERT INTO newsgroups (ng_name) VALUES ('" + name + "');";
  char* messageError;
  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

/** Borde funka */
void DiskDatabase::removeNewsGroup(long newsGroupId) {
  int exit = 0;

  string sql = "DELETE FROM newsgroups WHERE ng_id = " + to_string(newsGroupId) + ";";
  char* messageError;
  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

void DiskDatabase::addArticle(long newsGroupId, Article& article) {
  int exit = 0;
  string title = article.title;
  string text = article.content;
  string name = article.author;

  string sql = "INSERT INTO articles (at_title, at_text, au_name)"
               "VALUES ('" + title + "', '" + text + "', '" + name + "');"
               "INSERT INTO posted_articles VALUES (" + to_string(article.id) + ", " + to_string(newsGroupId) + ");";

  char* messageError;
  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
}

void DiskDatabase::removeArticle(long newsGroupId, long articleId) {
  int exit = 0;

  string sql = "DELETE FROM posted_articles WHERE "
               "at_id = " + to_string(articleId) + " AND ng_id = " + to_string(newsGroupId) + ";";

  char* messageError;
  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
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

bool DiskDatabase::exists(const char *filename) {
  ifstream ifile(filename);
  return ifile;
}
