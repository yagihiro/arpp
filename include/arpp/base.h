#pragma once

#include <memory>
#include <string>
#include "connection.h"
#include "schema.h"
#include "status.h"

namespace arpp {
class Base {
 public:
  Base();

  std::shared_ptr<Schema> schema() const;
  std::string table_name() const;

  virtual void migrate(std::shared_ptr<Connection> connection);

 protected:
  std::string _table_name;

 private:
  std::shared_ptr<Schema> _schema;

  /*
public:

  Status save();
  Status destroy();
  Status update();

  //find
  //limit
  //order
  //where
  //first
  //last
   */
};
}
