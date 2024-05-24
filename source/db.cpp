#include <sqlite3.h>
#include <potato/db.h>
#include <iostream>

using namespace std;
using namespace fantastic_potato;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i< argc; i++) {
    cout << azColName[i] << argv[i] <<endl;
  }
  return 0;
}

DB::DB(string name) {
  int rc;
  rc = sqlite3_open(name.c_str(), &db);
  if(rc) {
    cout << rc << endl;
    string msg = sqlite3_errmsg(db);
    cout << "Cannot open database::" << msg <<endl;
    sqlite3_close(db);
  }

}
DB::~DB() {
  int rc = sqlite3_close(db);
  if (rc != SQLITE_OK) {
    cout << "Cannot close database:" << sqlite3_errmsg(db) << endl;
  }
}
string DB::query(string sql) {

  int  rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cout << zErrMsg << endl;
    sqlite3_free(zErrMsg);
  }
  return "SUCCESS";
}
