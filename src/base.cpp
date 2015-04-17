#include <format.h>
#include "base.h"

namespace arpp {

Base::Base() { _schema = std::make_shared<Schema>(); }

std::shared_ptr<Schema> Base::schema() const { return _schema; }
std::string Base::table_name() const { return _schema->table_name(); }

void Base::migrate(std::shared_ptr<Connection> connection) {}
void Base::define_schema(std::shared_ptr<Schema> schema) {}

void Base::connect(std::shared_ptr<Connection> connection) {
  _connection = connection;

  // expect override
  define_schema(_schema);

  setup_fields();
}

Status Base::save() {
  if (_connection == nullptr) {
    return Status::status_ailment();
  }

  if (_new_record) {
    fmt::MemoryWriter buf;
    buf << "INSERT INTO " << table_name() << " (";

    std::vector<std::tuple<std::string, std::string> > values;

    for (auto &one : _fields) {
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

    _connection->execute_sql(buf.str());
  }

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
