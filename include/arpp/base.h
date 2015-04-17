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
  virtual ~Base() = default;

  std::shared_ptr<Schema> schema() const;
  std::string table_name() const;

  virtual void migrate(std::shared_ptr<Connection> connection);
  virtual void define_schema(std::shared_ptr<Schema> schema);

  std::string &operator[](const std::string &key) { return _fields.at(key); }
  const std::string &operator[](const std::string &key) const {
    return get_field(key);
  }
  const std::string &get_field(const std::string &key) const {
    return _fields.at(key);
  }
  void set_field(const std::string &key, const std::string &value) {
    _fields[key] = value;
  }
  void set_field(const std::pair<std::string, std::string> &pair) {
    set_field(pair.first, pair.second);
  }

  void connect(std::shared_ptr<Connection> connection);

  Status save();
  Status destroy();

 private:
  std::shared_ptr<Schema> _schema;
  std::map<std::string, std::string> _fields;
  std::shared_ptr<Connection> _connection;
  bool _new_record = true;

  void setup_fields();

  /*
public:

  Status update();

   */
};
}
