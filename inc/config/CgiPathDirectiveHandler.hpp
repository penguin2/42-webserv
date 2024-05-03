#ifndef CGI_PATH_DIRECTIVE_HANDLER_HPP
#define CGI_PATH_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class CgiPathDirectiveHandler : public ADirectiveHandler {
 public:
  CgiPathDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
