#pragma once
#include <functional>
#include <string>
#include <tuple>
#include <vector>

namespace arpp {

class Schema {
 public:
  class Property;
  using PropertyPtr = std::shared_ptr<Property>;

  class Property {
   public:
    static const int kUndefinedLimit = -1;

    Property();

    Property &set_limit(int size);
    Property &set_not_null();
    Property &set_unique();
    Property &set_primary_key();
    Property &set_auto_increment();

    int limit() const { return _limit; }
    bool not_null() const { return _not_null; }
    bool unique() const { return _unique; }
    bool primary_key() const { return _primary_key; }
    bool auto_increment() const { return _auto_increment; }

   private:
    int _limit = kUndefinedLimit;
    bool _not_null = false;
    bool _unique = false;
    bool _primary_key = false;
    bool _auto_increment = false;
  };

  enum {
    kColumnName = 0,
    kColumnType = 1,
    kColumnProperties = 2,
  };

  enum class Type {
    kInteger,
    kFloat,
    kString,
    kText,
    kBinary,
    kDateTime,
    kDate,
    kTime,
    kBoolean,
  };

  // you use kColumnName, kColumnType, kColumnProperties value for access
  // a std::get<>() template paramter
  using ColumnType = std::tuple<std::string, Type, PropertyPtr>;

  Schema();

  void define_table_name(const std::string &name);
  void define_column(const std::string &name, Type type,
                     const std::function<void(PropertyPtr)> &fn = nullptr);

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
