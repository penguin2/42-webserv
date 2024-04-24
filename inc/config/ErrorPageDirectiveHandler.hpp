#ifndef ERROR_PAGE_DIRECTIVE_HANDLER_HPP
#define ERROR_PAGE_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ErrorPageDirectiveHandler : public ADirectiveHandler {
 public:
  ErrorPageDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
