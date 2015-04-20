#include <format.h>
#include "base.h"

namespace arpp {

Base::Base() { _schema = std::make_shared<Schema>(); }

std::shared_ptr<Schema> Base::schema() const { return _schema; }
std::string Base::table_name() const { return _schema->table_name(); }

void Base::migrate(std::shared_ptr<Connection> connection,
                   std::shared_ptr<Schema> schema) {}
void Base::define_schema(std::shared_ptr<Schema> schema) {}

void Base::connect(std::shared_ptr<Connection> connection) {
  _connection = connection;

  _schema->define_column("id", "INTEGER PRIMARY KEY AUTOINCREMENT");

  // expect override
  define_schema(_schema);

  setup_fields();
}

void Base::loaded() { _new_record = false; }

Status Base::save() {
  if (_connection == nullptr) {
    return Status::status_ailment();
  }

  fmt::MemoryWriter buf;

  if (_new_record) {
    buf << "INSERT INTO " << table_name() << " (";

    std::vector<std::tuple<std::string, std::string> > values;

    for (auto &one : _fields) {
      if (one.first == "id") continue;
      values.push_back(std::make_tuple(one.first, one.second));
    }

    auto size = values.size();
    for (auto &one : values) {
      size -= 1;
      buf << std::get<0>(one);
      if (0 < size) {
        buf << ", ";
      }
    }

    buf << ") VALUES (";

    size = values.size();
    for (auto &one : values) {
      size -= 1;
      buf << "'" << std::get<1>(one) << "'";
      if (0 < size) {
        buf << ", ";
      }
    }

    buf << ");";
  } else {
    buf << "UPDATE " << table_name() << " SET ";

    auto size = _fields.size();
    for (auto &one : _fields) {
      size -= 1;

      if (one.first == "id") continue;

      buf << one.first << " = '" << one.second << "'";
      if (0 < size) {
        buf << ", ";
      }
    }

    buf << " WHERE id = " << _fields["id"];
  }

  _connection->execute_sql(buf.str());

  return Status::ok();
}

Status Base::destroy() {
  if (_connection == nullptr) {
    return Status::status_ailment();
  }

  std::string id;
  try {
    id = _fields.at("id");
  } catch (std::out_of_range &e) {
    return Status::status_ailment();
  }

  fmt::MemoryWriter buf;
  buf << "DELETE FROM " << table_name() << " WHERE id = " << id;

  _connection->execute_sql(buf.str());

  return Status::ok();
}

void Base::setup_fields() {
  _schema->each_column([&](const std::string &column_name) {
    _fields.emplace(std::make_pair(column_name, ""));
  });
}
}
