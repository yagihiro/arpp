#pragma once

#include <memory>
#include <vector>
#include "base.h"
#include "connection.h"
#include "status.h"

namespace arpp {

class Migration {
 public:
  typedef std::shared_ptr<Base> BasePointer;

  static Status migrate(std::shared_ptr<Connection> connection,
                        const std::vector<BasePointer> &bases);
};
}
