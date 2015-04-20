#pragma once
#include <functional>
#include <string>
#include <tuple>
#include <vector>

namespace arpp {

class Schema {
 public:
  enum {
    kColumnName = 0,
    kColumnType = 1,
  };

  // kColumnName, kColumnType
  typedef std::tuple<std::string, std::string> ColumnType;

  Schema();

  void define_table_name(const std::string &name);
  void define_column(const std::string &name, const std::string &type);

  std::string table_name() const;
  int defined_column_size() const;
  void each_column(
      const std::function<void(const std::string &column_name)> &fn);
  void each_define(const std::function<void(const ColumnType &def)> &fn);

 private:
  std::string _table_name;
  std::vector<ColumnType> _column_defs;
};
}
