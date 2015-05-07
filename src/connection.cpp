#include <arpp/arpp.h>
#include <format.h>
#include <SQLiteCpp/SQLiteCpp.h>

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

  Status execute_sql_for_each(
      const std::string &sql,
      const std::function<void(const Connection::RowType &)> &fn) {
    if (fn == nullptr) {
      return Status::status_ailment();
    }

    fmt::print("SQL: {}\n", sql);

    bool loaded = false;
    SQLite::Statement query(*_db, sql);
    while (query.executeStep()) {
      loaded = true;
      Connection::RowType row;
      auto count = query.getColumnCount();
      for (int i = 0; i < count; i++) {
        auto column = query.getColumn(i);
        auto t = column.getText();
        std::string value = (t) ? t : "";
        row.emplace(std::make_pair(column.getName(), value));
      }
      fn(row);
    }

    return (loaded) ? Status::ok() : Status::not_found();
  }

  Status drop_table(const std::string &table_name) {
    return execute_sql(fmt::format("DROP TABLE {}", table_name));
  }

  Status create_table(std::shared_ptr<Schema> schema) {
    fmt::MemoryWriter buf;

    buf << "CREATE TABLE " << schema->table_name() << " (";

    auto size = schema->defined_column_size();
    schema->each_define([&](const Schema::ColumnType &def) {
      size -= 1;
      buf << std::get<Schema::kColumnName>(def) << " "
          << column_type_to_string(std::get<Schema::kColumnType>(def));
      auto prop =
          column_prop_to_string(std::get<Schema::kColumnProperties>(def));
      if (!prop.empty()) {
        buf << " " << prop;
      }
      if (0 < size) {
        buf << ", ";
      }
    });

    buf << ")";

    return execute_sql(buf.str());
  }

  Status transaction(const std::function<Status()> &t) {
    if (t == nullptr) return Status::invalid_argument();

    SQLite::Transaction transaction(*_db);
    auto status = t();
    if (status.is_ok()) transaction.commit();

    return Status::ok();
  }

  int64_t last_row_id() const { return _db->getLastInsertRowid(); }

 private:
  std::unique_ptr<SQLite::Database> _db;

  std::string column_type_to_string(Schema::Type type) {
    static std::map<Schema::Type, std::string> mapping = {
        {Schema::Type::kInteger, "INTEGER"},
        {Schema::Type::kBoolean, "INTEGER"},
        {Schema::Type::kFloat, "REAL"},
        {Schema::Type::kString, "TEXT"},
        {Schema::Type::kText, "TEXT"},
        {Schema::Type::kDateTime, "NUMERIC"},
        {Schema::Type::kDate, "NUMERIC"},
        {Schema::Type::kTime, "NUMERIC"},
        {Schema::Type::kBinary, "BLOB"},
    };

    return mapping[type];
  }

  std::string column_prop_to_string(Schema::PropertyPtr prop) {
    std::vector<std::string> results;
    if (prop->not_null()) {
      results.emplace_back("NOT NULL");
    }
    if (prop->unique()) {
      results.emplace_back("UNIQUE");
    }
    if (prop->primary_key()) {
      results.emplace_back("PRIMARY KEY");
    }
    if (prop->auto_increment()) {
      results.emplace_back("AUTOINCREMENT");
    }

    fmt::MemoryWriter buf;
    auto size = results.size();
    for (auto &one : results) {
      size -= 1;
      buf << one;
      if (0 < size) {
        buf << " ";
      }
    }

    return buf.str();
  }
};

const std::string Connection::kOptionAdapter = "adapter";
const std::string Connection::kOptionDatabase = "database";

static std::shared_ptr<Connection> _shared_connection;

std::shared_ptr<Connection> Connection::connect(
    const std::map<std::string, std::string> &options, Status *status) {
  if (_shared_connection == nullptr) {
    std::shared_ptr<Connection> p = std::make_shared<Connection>();
    p->set_options(options);
    *status = p->connect();
    _shared_connection = p;
  }

  return _shared_connection;
}

std::shared_ptr<Connection> Connection::shared_connection() {
  return _shared_connection;
}

bool Connection::has_connected() { return _shared_connection != nullptr; }

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

Status Connection::execute_sql_for_each(
    const std::string &sql, const std::function<void(const RowType &)> &fn) {
  return _impl->execute_sql_for_each(sql, fn);
}

Status Connection::drop_table(const std::string &table_name) {
  return _impl->drop_table(table_name);
}

Status Connection::create_table(std::shared_ptr<Schema> schema) {
  return _impl->create_table(schema);
}

Status Connection::transaction(const std::function<Status()> &t) {
  return _impl->transaction(t);
}

int64_t Connection::last_row_id() const { return _impl->last_row_id(); }

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
