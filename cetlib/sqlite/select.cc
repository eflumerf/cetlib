#include "cetlib/sqlite/select.h"

#include <cassert>

namespace {
  int getResult(void* data, int ncols, char** results, char** cnames)
  {
    assert(ncols >= 1);
    auto j = static_cast<sqlite::query_result*>(data);
    if (j->columns.empty()) {
      for (int i{}; i < ncols ; ++i)
        j->columns.push_back(cnames[i]);
    }

    sqlite::stringstream resultStream;
    for (int i{}; i < ncols ; ++i)
      resultStream << results[i];
    j->data.emplace_back(std::move(resultStream));
    return 0;
  }
}

namespace sqlite {

  query_result query(sqlite3* db, std::string const& ddl)
  {
    query_result res;
    char* errmsg {nullptr};
    if (sqlite3_exec(db, ddl.c_str(), getResult, &res, &errmsg) != SQLITE_OK) {
      std::string msg{errmsg};
      sqlite3_free(errmsg);
      throw sqlite::Exception(sqlite::errors::SQLExecutionError, msg);
    }
    return res;
  }

  void operator<<(query_result& r, CompleteQuery const& cq)
  {
    r = query(cq.db_, cq.ddl_+";");
  }

}