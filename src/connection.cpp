#include <SQLiteCpp/SQLiteCpp.h>
#include <format.h>
#include "connection.h"

namespace arpp {

class Connection::Impl {
 public:
  Impl(const std::string &path) {
    _db.reset(
        new SQLite::Database(path, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE));
  }

  bool exists_table(const std::string &table_name) const {
    return _db->tableExists(table_name);
  }

  Status execute_sql(const std::string &sql) {
    fmt::print("SQL: {}\n", sql);
    _db->exec(sql);
    return Status::ok();
  }

  Status transaction(const std::function<Status()> &t) {
    if (t == nullptr) return Status::invalid_argument();

    SQLite::Transaction transaction(*_db);
    auto status = t();
    if (status.is_ok()) transaction.commit();

    return Status::ok();
  }

 private:
  std::unique_ptr<SQLite::Database> _db;
};

const std::string Connection::kOptionAdapter = "adapter";
const std::string Connection::kOptionDatabase = "database";

std::shared_ptr<Connection> Connection::connect(
    const std::map<std::string, std::string> &options, Status *status) {
  std::shared_ptr<Connection> p = std::make_shared<Connection>();
  p->set_options(options);
  *status = p->connect();
  return p;
}

Connection::Connection() {}

const std::map<std::string, std::string> &Connection::options() const {
  return std::move(_options);
}

bool Connection::exists_table(const std::string &table_name) const {
  return _impl->exists_table(table_name);
}

Status Connection::execute_sql(const std::string &sql) {
  return _impl->execute_sql(sql);
}

Status Connection::transaction(const std::function<Status()> &t) {
  return _impl->transaction(t);
}

void Connection::set_options(
    const std::map<std::string, std::string> &options) {
  _options = std::move(options);
}

Status Connection::connect() {
  if (_options["database"].size() == 0) {
    return Status::not_found();
  }

  _impl.reset(new Impl(_options["database"]));

  return Status::ok();
}
}
