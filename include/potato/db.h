#pragma once

#include <string>
#include <sqlite3.h>


using namespace std;
namespace fantastic_potato {
  class SchwabDB {
    string name;
    sqlite3 *db;
    char *zErrMsg;
    static int callback(void *data, int argc, char **argv, char **azColName);
    public:
    SchwabDB(string name);
    ~SchwabDB();
    string query(string sql);
    string getValueByType(string type);
  };
}
