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

  std::string &operator[](const std::string &key) {}

 protected:
  std::string _table_name;
  std::map<std::string, std::string> _value;

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
