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

  virtual void migrate(std::shared_ptr<Connection> connection,
                       std::shared_ptr<Schema> schema);
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

  void loaded();

  Status save();
  Status destroy();

  // callback on INSERT and UPDATE
  virtual void before_save();
  // callback on INSERT and UPDATE
  virtual void after_save();
  // callback on INSERT
  virtual void before_create();
  // callback on INSERT
  virtual void after_create();
  // callback on UPDATE
  virtual void before_update();
  // callback on UPDATE
  virtual void after_update();

  std::string to_json() const;

 private:
  std::shared_ptr<Schema> _schema;
  std::map<std::string, std::string> _fields;
  std::shared_ptr<Connection> _connection;
  bool _new_record = true;

  void setup_fields();
  void set_created_at();
  void set_updated_at();
  std::string now_string() const;
};
}
