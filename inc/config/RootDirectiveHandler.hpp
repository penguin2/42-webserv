#ifndef ROOT_DIRECTIVE_HANDLER_HPP
#define ROOT_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class RootDirectiveHandler : public ADirectiveHandler {
 public:
  RootDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
