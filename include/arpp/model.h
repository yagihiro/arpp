#pragma once

#include <memory>
#include "connection.h"

namespace arpp {

template <typename T>
class Model {
 public:
  static std::shared_ptr<T> create() {
    auto m = std::make_shared<T>();

    m->connect(Connection::shared_connection());

    return m;
  }
};
}
