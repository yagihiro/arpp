#include <arpp/arpp.h>

namespace arpp {

static std::shared_ptr<ConnectionBundle> _shared_bundle;

std::shared_ptr<ConnectionBundle> ConnectionBundle::shared_bundle() {
  return _shared_bundle;
}
}
