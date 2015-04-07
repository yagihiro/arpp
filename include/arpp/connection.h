#pragma once

#include <map>
#include <memory>
#include <string>
#include "status.h"

namespace gummy {
  
  namespace model {
    
    class Connection {
    public:
      
      static Status connect(const std::map<std::string, std::string> &options, Connection **outp);
      
      Connection();
      
    private:
      
      void set_options(const std::map<std::string, std::string> &options);
      Status connect();

      std::map<std::string, std::string> _options;
      
      class Impl;
      std::unique_ptr<Impl> _impl;
      
    };
    
  }
  
}

