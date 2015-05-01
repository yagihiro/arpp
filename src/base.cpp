#include <arpp/arpp.h>
#include <ctime>
#include <format.h>

namespace arpp {

Base::Base() { _schema = std::make_shared<Schema>(); }

std::shared_ptr<Schema> Base::schema() const { return _schema; }
std::string Base::table_name() const { return _schema->table_name(); }

void Base::migrate(std::shared_ptr<Connection> connection,
                   std::shared_ptr<Schema> schema) {}
void Base::define_schema(std::shared_ptr<Schema> schema) {}

void Base::connect(std::shared_ptr<Connection> connection) {
  _connection = connection;

  // expect override
  define_schema(_schema);

  setup_fields();
}

void Base::loaded() { _new_record = false; }

Status Base::save() {
  if (_connection == nullptr) {
    return Status::status_ailment();
  }

  before_save();
  set_updated_at();

  fmt::MemoryWriter buf;

  if (_new_record) {
    before_create();
    set_created_at();

    buf << "INSERT INTO " << table_name() << " (";

    std::vector<std::tuple<std::string, std::string> > values;

    for (auto &one : _fields) {
      if (one.first == "id") continue;
      values.push_back(std::make_tuple(one.first, one.second));
    }

    auto size = values.size();
    for (auto &one : values) {
      size -= 1;
      buf << std::get<0>(one);
      if (0 < size) {
        buf << ", ";
      }
    }

    buf << ") VALUES (";

    size = values.size();
    for (auto &one : values) {
      size -= 1;
      buf << "'" << std::get<1>(one) << "'";
      if (0 < size) {
        buf << ", ";
      }
    }

    buf << ");";
  } else {
    before_update();

    buf << "UPDATE " << table_name() << " SET ";

    auto size = _fields.size();
    for (auto &one : _fields) {
      size -= 1;

      if (one.first == "id") continue;

      buf << one.first << " = '" << one.second << "'";
      if (0 < size) {
        buf << ", ";
      }
    }

    buf << " WHERE id = " << _fields["id"];
  }

  _connection->execute_sql(buf.str());

  if (_new_record) {
    auto id = _connection->last_row_id();
    set_field("id", fmt::format("{0}", id));
    after_create();
  } else {
    after_update();
  }

  after_save();

  return Status::ok();
}

Status Base::destroy() {
  if (_connection == nullptr) {
    return Status::status_ailment();
  }

  std::string id;
  try {
    id = _fields.at("id");
  } catch (std::out_of_range &e) {
    return Status::status_ailment();
  }

  fmt::MemoryWriter buf;
  buf << "DELETE FROM " << table_name() << " WHERE id = " << id;

  _connection->execute_sql(buf.str());

  return Status::ok();
}

void Base::before_save() {}

void Base::after_save() {}

void Base::before_create() {}

void Base::after_create() {}

void Base::before_update() {}

void Base::after_update() {}

std::string Base::to_json() const {
  fmt::MemoryWriter w;

  w << "{";
  auto size = _fields.size();
  for (auto &one : _fields) {
    size -= 1;
    w << "\"" << one.first << "\":\"" << one.second << "\"";
    if (0 < size) {
      w << ",";
    }
  }
  w << "}";

  return std::move(w.str());
}

void Base::setup_fields() {
  _schema->each_column([&](const std::string &column_name) {
    _fields.emplace(std::make_pair(column_name, ""));
  });
}

void Base::set_created_at() {
  set_field("created_at", fmt::format("{0}", now_string()));
}

void Base::set_updated_at() {
  set_field("updated_at", fmt::format("{0}", now_string()));
}

std::string Base::now_string() const {
  auto now = std::time(nullptr);
  auto tm = gmtime(&now);
  char buf[sizeof("2000-01-01T00:00:00Z")];
  std::strftime(buf, sizeof(buf), "%FT%TZ", tm);

  return buf;
}
}
