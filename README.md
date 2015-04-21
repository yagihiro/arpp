# arpp

Active Record library for C++11

# Build Status

TBD

# Documentation

## Setting up

An example for connect to sqlite db

    std::map<std::string, std::string> options = {
      {arpp::Connection::kOptionAdapter, "sqlite3"},
      {arpp::Connection::kOptionDatabase, "db.sqlite3"},
    };

    arpp::Status status;
    auto c = arpp::Connection::connect(options, &status);
    if (!status.is_ok()) {
      // error sequence
    }

An example for migration

    // class definition
    class SampleModel : public arpp::Base {
     public:
      SampleModel() {}

      virtual void define_schema(std::shared_ptr<arpp::Schema> schema) override {
        schema->define_table_name("simples");
        schema->define_column("value", "TEXT");
      }

      virtual void migrate(std::shared_ptr<arpp::Connection> connection,
                           std::shared_ptr<arpp::Schema> schema) override {
        auto status = connection->transaction([&]() -> arpp::Status {
          if (connection->exists_table("simples")) {
            connection->drop_table("simples");
          }
          connection->create_table(schema);
          return arpp::Status::ok();
        });
        if (!status.is_ok()) {
          // error sequence
        }
      }
    };

    // migration code
    arpp::Migration::migrate(c, {arpp::Model<SampleModel>::create()});

## CRUD

### Read

first one:

    auto model = arpp::Query<SampleModel>::first();

last one:

    auto model = arpp::Query<SampleModel>::last();

find by id:

    auto model = arpp::Query<SampleModel>::find(1);

find by 'where' query:

    auto models = arpp::Query<SampleModel>::where("id < {}", 10)
                    ->limit(3)
                    ->order("id", arpp::Order::kDesc)
                    ->load();

### Create

insert one:

    auto m = arpp::Model<SampleModel>::create();
    m->set_field("value", "hoge");
    auto status = m->save();
    if (!status.is_ok()) {
      // error sequence
    }

### Update

update one:

    auto model = arpp::Query<SampleModel>::find(2);
    model->set_field("value", "new_value");
    model->save();    // update

    model = arpp::Query<SampleModel>::find(2);
    // model is id=2 / value=new_value

### Delete

delete one:

    auto model = arpp::Query<SampleModel>::find(1);
    model->destroy();


# Installation

## Requirement

* Mac OS X: Tested on "Apple LLVM version 6.0 (clang-600.0.57) (based on LLVM 3.5svn)"
* Unix: no test


# License

MIT License
