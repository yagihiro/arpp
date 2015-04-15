#pragma once

#include <map>
#include <memory>
#include <string>
#include "status.h"

namespace arpp {

class Connection {
 public:
  static const std::string kOptionAdapter;
  static const std::string kOptionDatabase;

  static std::shared_ptr<Connection> connect(
      const std::map<std::string, std::string> &options, Status *status);

  static std::shared_ptr<Connection> shared_connection();

  static bool has_connected();

  Connection();

  const std::map<std::string, std::string> &options() const;
  bool exists_table(const std::string &table_name) const;
  Status execute_sql(const std::string &sql);

  typedef std::map<std::string, std::string> RowType;
  Status execute_sql_for_each(const std::string &sql,
                              const std::function<void(const RowType &)> &fn);
  Status transaction(const std::function<Status()> &t);

 private:
  void set_options(const std::map<std::string, std::string> &options);
  Status connect();

  std::map<std::string, std::string> _options;

  class Impl;
  std::unique_ptr<Impl> _impl;
};
}
