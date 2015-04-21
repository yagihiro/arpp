#include <arpp/arpp.h>

namespace arpp {

Schema::Schema() {}

void Schema::define_table_name(const std::string &name) { _table_name = name; }

void Schema::define_column(const std::string &name, const std::string &type) {
  auto column = std::make_tuple(name, type);
  _column_defs.push_back(column);
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
