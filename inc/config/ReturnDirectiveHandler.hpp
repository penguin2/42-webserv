#ifndef RETURN_DIRECTIVE_HANDLER_HPP
#define RETURN_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ReturnDirectiveHandler : public ADirectiveHandler {
 public:
  ReturnDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
