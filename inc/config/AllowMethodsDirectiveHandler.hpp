#ifndef ALLOW_METHODS_DIRECTIVE_HANDLER_HPP
#define ALLOW_METHODS_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class AllowMethodsDirectiveHandler : public ADirectiveHandler {
 public:
  AllowMethodsDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
