#ifndef CGI_EXT_DIRECTIVE_HANDLER_HPP
#define CGI_EXT_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class CgiExtDirectiveHandler : public ADirectiveHandler {
 public:
  CgiExtDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
