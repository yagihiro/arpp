#pragma once

#include <map>
#include <memory>
#include <string>
#include "status.h"

namespace arpp {

class Connection {
public:
  static std::shared_ptr<Connection> connect(const std::map<std::string, std::string> &options, Status *status);

  Connection();

private:
  void set_options(const std::map<std::string, std::string> &options);
  Status connect();

  std::map<std::string, std::string> _options;

  class Impl;
  std::unique_ptr<Impl> _impl;
};
}
