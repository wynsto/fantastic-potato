#pragma once

#include <string>
#include <sqlite3.h>


using namespace std;
namespace schwab_personal_trading {
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
