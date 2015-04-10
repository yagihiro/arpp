#include "base.h"

namespace arpp {

Base::Base() { _schema = std::make_shared<Schema>(); }

std::shared_ptr<Schema> Base::schema() const { return _schema; }
}
