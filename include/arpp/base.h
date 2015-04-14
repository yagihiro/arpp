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
  virtual void define_schema(std::shared_ptr<Schema> schema);

  std::string &operator[](const std::string &key) { return _fields.at(key); }
  const std::string &operator[](const std::string &key) const {
    return _fields.at(key);
  }

  void connect(std::shared_ptr<Connection> connection);

  Status save();

 private:
  std::shared_ptr<Schema> _schema;
  std::map<std::string, std::string> _fields;
  std::shared_ptr<Connection> _connection;
  bool _new_record = true;

  void setup_fields();

  /*
public:

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
