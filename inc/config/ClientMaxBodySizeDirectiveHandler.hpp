#ifndef CLIENT_MAX_BODY_SIZE_DIRECTIVE_HANDLER_HPP
#define CLIENT_MAX_BODY_SIZE_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ClientMaxBodySizeDirectiveHandler : public ADirectiveHandler {
 public:
  ClientMaxBodySizeDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
