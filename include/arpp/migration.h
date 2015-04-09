#pragma once

#include <memory>
#include <vector>
#include "connection.h"
#include "schema.h"
#include "status.h"

namespace arpp {
  
  class Migration {
  public:
    static Status migrate(std::shared_ptr<Connection> connection, const std::vector<Schema> &schemas);
  };
}
