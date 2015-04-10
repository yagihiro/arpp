#include "status.h"

namespace arpp {

Status::Status() {}

Status Status::ok(const std::string &msg) {
  return std::move(Status(kOk, msg));
}

Status Status::not_found(const std::string &msg) {
  return std::move(Status(kNotFound, msg));
}

Status::Status(Code code, const std::string &msg)
    : _code(code), _message(msg) {}
}
