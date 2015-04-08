#include <SQLiteCpp/SQLiteCpp.h>
#include "connection.h"

namespace arpp {
  
  class Connection::Impl {
  public:
    Impl(const std::string &path) {
      _db.reset(new SQLite::Database(path, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE));
    }
    
  private:
    
    std::unique_ptr<SQLite::Database> _db;
    
  };
  
  Status Connection::connect(const std::map<std::string, std::string> &options, Connection **outp) {
    Connection *p = *outp;
    
    p = new Connection;
    p->set_options(options);
    return std::move(p->connect());
  }
  
  Connection::Connection() {
    
  }
  
  void Connection::set_options(const std::map<std::string, std::string> &options) {
    _options = std::move(options);
  }
  
  Status Connection::connect() {
    if (_options["database"].size() == 0) {
      return Status::not_found();
    }
    
    _impl.reset(new Impl(_options["database"]));
    
    return Status::ok();
  }
  
}

