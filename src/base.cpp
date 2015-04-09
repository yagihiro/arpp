#include "base.h"

namespace arpp {
  
  Base::Base()
  : _schema() {
    
  }
  
  std::shared_ptr<Schema> Base::schema() const {
    return _schema;
  }
  
  
}

