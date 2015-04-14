#include "schema.h"

namespace arpp {

Schema::Schema() {}

void Schema::define_table_name(const std::string &name) { _table_name = name; }

void Schema::define_column(const std::string &name, const std::string &type) {
  auto column = std::make_tuple(name, type);
  _column_defs.push_back(column);
}

std::string Schema::table_name() const { return _table_name; }
void Schema::each_column(
    const std::function<void(const std::string &column_name)> &fn) {
  if (fn == nullptr) {
    return;
  }
  for (auto &one : _column_defs) {
    fn(std::get<kColumnName>(one));
  }
}
}
