#include <arpp/arpp.h>

namespace arpp {

Schema::Property::Property() {}

Schema::PropertyPtr Schema::Property::limit(int size) {
  auto new_prop = std::make_shared<Schema::Property>(*this);
  new_prop->_limit = size;
  return new_prop;
}

Schema::PropertyPtr Schema::Property::null() {
  auto new_prop = std::make_shared<Schema::Property>(*this);
  new_prop->_null = true;
  return new_prop;
}

Schema::PropertyPtr Schema::Property::unique() {
  auto new_prop = std::make_shared<Schema::Property>(*this);
  new_prop->_unique = true;
  return new_prop;
}

Schema::PropertyPtr Schema::Property::primary_key() {
  auto new_prop = std::make_shared<Schema::Property>(*this);
  new_prop->_primary_key = true;
  return new_prop;
}

Schema::PropertyPtr Schema::Property::auto_increment() {
  auto new_prop = std::make_shared<Schema::Property>(*this);
  new_prop->_auto_increment = true;
  return new_prop;
}

Schema::Schema() {
  auto id_prop = std::make_shared<Property>()->primary_key()->auto_increment();
  _column_defs.emplace_back(std::make_tuple("id", Type::kInteger, id_prop));
}

void Schema::define_table_name(const std::string &name) { _table_name = name; }

Schema::PropertyPtr Schema::define_column(const std::string &name, Type type) {
  auto prop = std::make_shared<Property>();
  _column_defs.emplace_back(std::make_tuple(name, type, prop));
  return prop;
}

std::string Schema::table_name() const { return _table_name; }
int Schema::defined_column_size() const {
  return static_cast<int>(_column_defs.size());
}

void Schema::each_column(
    const std::function<void(const std::string &column_name)> &fn) {
  if (fn == nullptr) {
    return;
  }
  each_define([&](const ColumnType &def) { fn(std::get<kColumnName>(def)); });
}

void Schema::each_define(const std::function<void(const ColumnType &def)> &fn) {
  if (fn == nullptr) {
    return;
  }
  for (auto &one : _column_defs) {
    fn(one);
  }
}
}
