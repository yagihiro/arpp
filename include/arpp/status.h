#pragma once

#include <string>

namespace arpp {

class Status {
 public:
  Status();
  static Status ok(const std::string &msg = "");
  static Status not_found(const std::string &msg = "");
  static Status invalid_argument(const std::string &msg = "");
  static Status status_ailment(const std::string &msg = "");

  inline bool is_ok() const { return _code == kOk; }
  inline bool is_not_found() const { return _code == kNotFound; }
  inline bool is_invalid_argument() const { return _code == kInvalidArgument; }
  inline bool is_status_ailment() const { return _code == kStatusAilment; }

 private:
  enum Code {
    kOk = 0,
    kNotFound = 1,
    kInvalidArgument = 2,
    kStatusAilment = 3,
  };

  Code _code = kOk;
  std::string _message;

  Status(Code code, const std::string &msg);
};
}
