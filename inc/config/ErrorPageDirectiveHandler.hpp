#ifndef ERROR_PAGE_DIRECTIVE_HANDLER_HPP
#define ERROR_PAGE_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ErrorPageDirectiveHandler : public ADirectiveHandler {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif
