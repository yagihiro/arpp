#pragma once

#include <memory>
#include "schema.h"
#include "status.h"

namespace arpp {
class Base {
public:

  Base();
  
  std::shared_ptr<Schema> schema() const;
  
  virtual void set_schema(std::shared_ptr<Schema> schema) = 0;

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
