#pragma once
#include <format.h>
#include <memory>
#include "connection.h"

namespace arpp {

template <typename Model>
class Query {
 public:
  typedef std::shared_ptr<Model> ModelPtr;

  static ModelPtr first() {
    if (!Connection::has_connected()) {
      return nullptr;
    }

    auto m = std::make_shared<Model>();
    m->connect(Connection::shared_connection());

    fmt::MemoryWriter buf;
    buf << "SELECT * FROM " << m->table_name() << " ORDER BY id ASC LIMIT 1;";

    auto c = Connection::shared_connection();
    c->execute_sql_for_each(buf.str(), [&](const Connection::RowType &row) {
      for (auto one : row) {
        m->set_field(one);
      }
    });

    return m;
  }
};
}
