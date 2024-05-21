#pragma once

#include <string>

using namespace std;
namespace fantastic_potato {
  class DB {
    string name;
    public:
    DB(string name);
    string query(string sql);
  };
}
