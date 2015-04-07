#pragma once

#include <string>

namespace arpp {

class Status {
public:
  Status();
  static Status ok(const std::string &msg = "");
  static Status not_found(const std::string &msg = "");

  inline bool is_ok() const { return _code == kOk; }

  inline bool is_not_found() const { return _code == kNotFound; }

private:
  enum Code {
    kOk = 0,
    kNotFound = 1,
  };

  Code _code = kOk;
  std::string _message;

  Status(Code code, const std::string &msg);
};
}
