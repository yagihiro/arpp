#include <arpp/arpp.h>

namespace arpp {

Schema::Property::Property() {}

Schema::Property &Schema::Property::set_limit(int size) {
  _limit = size;
  return *this;
}

Schema::Property &Schema::Property::set_not_null() {
  _not_null = true;
  return *this;
}

Schema::Property &Schema::Property::set_unique() {
  _unique = true;
  return *this;
}

Schema::Property &Schema::Property::set_primary_key() {
  _primary_key = true;
  return *this;
}

Schema::Property &Schema::Property::set_auto_increment() {
  _auto_increment = true;
  return *this;
}

Schema::Schema() {
  define_column("id", Type::kInteger, [&](PropertyPtr prop) {
    prop->set_primary_key().set_auto_increment();
  });
}

void Schema::define_table_name(const std::string &name) { _table_name = name; }

void Schema::define_column(const std::string &name, Type type,
                           const std::function<void(PropertyPtr)> &fn) {
  auto prop = std::make_shared<Property>();
  _column_defs.emplace_back(std::make_tuple(name, type, prop));
  if (fn) {
    fn(prop);
  }
}

void Schema::define_timestamps() {
  define_column("created_at", Type::kDateTime);
  define_column("updated_at", Type::kDateTime);
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
