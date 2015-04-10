#include "base.h"

namespace arpp {

Base::Base() { _schema = std::make_shared<Schema>(); }

std::shared_ptr<Schema> Base::schema() const { return _schema; }
std::string Base::table_name() const { return _table_name; }

void Base::migrate(std::shared_ptr<Connection> connection) {}
}
