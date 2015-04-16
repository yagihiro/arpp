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
    return _exec_single([&](ModelPtr m, fmt::MemoryWriter &mw) {
      mw << "SELECT * FROM " << m->table_name() << " ORDER BY id ASC LIMIT 1;";
    });
  }

  static ModelPtr last() {
    return _exec_single([&](ModelPtr m, fmt::MemoryWriter &mw) {
      mw << "SELECT * FROM " << m->table_name() << " ORDER BY id DESC LIMIT 1;";
    });
  }

  static ModelPtr find(int id) {
    return _exec_single([&](ModelPtr m, fmt::MemoryWriter &mw) {
      mw << "SELECT * FROM " << m->table_name() << " WHERE id = " << id
         << " LIMIT 1;";
    });
  }

 private:
  static ModelPtr _exec_single(
      const std::function<void(ModelPtr, fmt::MemoryWriter &)> &fn) {
    if (!Connection::has_connected()) {
      return nullptr;
    }

    auto m = std::make_shared<Model>();
    m->connect(Connection::shared_connection());

    fmt::MemoryWriter buf;
    fn(m, buf);

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
