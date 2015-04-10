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

  Connection();

  const std::map<std::string, std::string> &options() const;
  bool exists_table(const std::string &table_name) const;
  Status execute_sql(const std::string &sql);
  Status transaction(const std::function<Status()> &t);

 private:
  void set_options(const std::map<std::string, std::string> &options);
  Status connect();

  std::map<std::string, std::string> _options;

  class Impl;
  std::unique_ptr<Impl> _impl;
};
}
