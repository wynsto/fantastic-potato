#pragma once

#include <string>
#include <sqlite3.h>


using namespace std;
namespace fantastic_potato {
  class DB {
    string name;
    sqlite3 *db;
    char *zErrMsg;
    static int callback(void *data, int argc, char **argv, char **azColName);
    public:
    DB(string name);
    ~DB();
    string query(string sql);
  };
}
