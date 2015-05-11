#pragma once
#include <memory>

namespace arpp {

/**
 * Connection Bundle classs for database partitioning.
 */
class ConnectionBundle {
 public:
  static std::shared_ptr<ConnectionBundle> shared_bundle();
};
}
