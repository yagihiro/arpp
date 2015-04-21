#include <arpp/arpp.h>

namespace arpp {

Status Migration::migrate(std::shared_ptr<Connection> connection,
                          const std::vector<BasePointer> &bases) {
  auto options = connection->options();
  auto database = options["database"];

  for (auto &base : bases) {
    base->migrate(connection, base->schema());
  }

  return Status::ok();
}
}
