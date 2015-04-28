#pragma once
#include <format.h>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include "connection.h"
#include "model.h"

namespace arpp {

enum class Order {
  kAsc = 0,
  kDesc = 1,
};

template <typename T>
class Project {
 public:
  using ModelPtr = std::shared_ptr<T>;
  using ProjectType = Project<T>;
  using ProjectPtr = std::shared_ptr<ProjectType>;

  enum class Type { kWhere, kOrder, kLimit };

  Project(Type type, const std::string &exp) { add_condition(type, exp); }

  std::vector<ModelPtr> load() {
    if (!Connection::has_connected()) {
      return std::move(std::vector<ModelPtr>());
    }

    auto for_schema = Model<T>::create();

    fmt::MemoryWriter buf;
    buf << "SELECT * FROM " << for_schema->table_name();
    for (auto &one : _conditions) {
      if (one.first == Type::kWhere) {
        buf << " WHERE " << one.second;
      } else if (one.first == Type::kOrder) {
        buf << " ORDER BY " << one.second;
      } else if (one.first == Type::kLimit) {
        buf << " LIMIT " << one.second;
      }
    }

    std::vector<ModelPtr> models;
    auto c = Connection::shared_connection();
    c->execute_sql_for_each(buf.str(), [&](const Connection::RowType &row) {
      auto m = Model<T>::create();
      for (auto one : row) {
        m->set_field(one);
      }
      m->loaded();
      models.emplace_back(m);
    });

    return std::move(models);
  }

  ProjectPtr limit(int n) {
    auto q = fmt::format("{}", n);
    auto new_project = std::make_shared<ProjectType>(*this);
    new_project->add_condition(Type::kLimit, q);
    return new_project;
  }

  ProjectPtr order(const std::string &column, Order o) {
    static std::string order_str[2] = {"ASC", "DESC"};
    auto oi = static_cast<int>(o);
    auto new_project = std::make_shared<ProjectType>(*this);
    std::string cond = fmt::format("{} {}", column, order_str[oi]);
    new_project->add_condition(Type::kOrder, cond);
    return new_project;
  }

 private:
  std::map<Type, std::string> _conditions;

  void add_condition(Type type, const std::string &exp) {
    _conditions.insert(std::make_pair(type, exp));
  }
};

template <typename T>
class Query {
 public:
  using ModelPtr = std::shared_ptr<T>;
  using ProjectType = Project<T>;
  using ProjectPtr = std::shared_ptr<ProjectType>;

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

  template <typename... Args>
  static ProjectPtr where(const std::string &cond, const Args &... args) {
    auto q = fmt::format(cond, args...);

    return std::make_shared<ProjectType>(ProjectType::Type::kWhere, q);
  }

  static ProjectPtr limit(int n) {
    auto q = fmt::format("{}", n);
    return std::make_shared<ProjectType>(ProjectType::Type::kLimit, q);
  }

  static ProjectPtr order(const std::string &column, Order o) {
    fmt::MemoryWriter w;
    w << column << " ";
    if (o == Order::kAsc) {
      w << "ASC";
    } else {
      w << "DESC";
    }
    return std::make_shared<ProjectType>(ProjectType::Type::kOrder, w.str());
  }

 private:
  static ModelPtr _exec_single(
      const std::function<void(ModelPtr, fmt::MemoryWriter &)> &fn) {
    if (!Connection::has_connected()) {
      return nullptr;
    }

    auto m = Model<T>::create();

    fmt::MemoryWriter buf;
    fn(m, buf);

    auto c = Connection::shared_connection();
    c->execute_sql_for_each(buf.str(), [&](const Connection::RowType &row) {
      for (auto one : row) {
        m->set_field(one);
      }
    });

    m->loaded();

    return m;
  }
};
}
