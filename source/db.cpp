#include <schwab/db.h>
#include <sqlite3.h>

#include <iostream>
#include "db.h"

using namespace std;
using namespace schwab_personal_trading;

int SchwabDB::callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    cout << azColName[i] << argv[i] << endl;
  }
  return 0;
}

SchwabDB::SchwabDB(string name) {
  int rc;
  rc = sqlite3_open(name.c_str(), &db);
  if (rc) {
    cout << rc << endl;
    string msg = sqlite3_errmsg(db);
    cout << "Cannot open database::" << msg << endl;
    sqlite3_close(db);
  }
}
SchwabDB::~SchwabDB() {
  int rc = sqlite3_close(db);
  if (rc != SQLITE_OK) {
    cout << "Cannot close database:" << sqlite3_errmsg(db) << endl;
  }
}
string SchwabDB::query(string sql) {
  sqlite3_stmt *stmt = NULL;  // stmt语句句柄

  // 进行查询前的准备工作——检查语句合法性
  //-1代表系统会自动计算SQL语句的长度
  int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

  if (result == SQLITE_OK) {
    clog << "查询语句OK";
    // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // 取出第0列字段的值
      const unsigned char *type = sqlite3_column_text(stmt, 0);
      // 取出第1列字段的值
      int value = sqlite3_column_int(stmt, 1);
      // 输出相关查询的数据
      clog << "type = " << type << ", value = " << value;
    }
  } else {
    clog << "查询语句有问题";
  }
  // 清理语句句柄，准备执行下一个语句
  sqlite3_finalize(stmt);

  int rc = sqlite3_exec(db, sql.c_str(), SchwabDB::callback, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cout << zErrMsg << endl;
    sqlite3_free(zErrMsg);
  }
  return "SUCCESS";
}

string SchwabDB::getValueByType(string type) {
  string sql = "select value from schwab_kv where type = '" + type + "';";
  // clog << "sql = " << sql;
  sqlite3_stmt *stmt = NULL;  // stmt语句句柄
  int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
  string value = "";
  if (result == SQLITE_OK) {
    // clog << "查询语句OK";
    // 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // 取出第1列字段的值
      value = string(reinterpret_cast< const char* >(sqlite3_column_text(stmt, 0)));
      // 输出相关查询的数据
      // clog << "type = " << type << ", value = " << value;
    }
  } else {
    // clog << "查询语句有问题";
  }
  // 清理语句句柄，准备执行下一个语句
  sqlite3_finalize(stmt);
  return value;
}
