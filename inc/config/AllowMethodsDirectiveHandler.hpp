#ifndef ALLOW_METHODS_DIRECTIVE_HANDLER_HPP
#define ALLOW_METHODS_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class AllowMethodsDirectiveHandler : public ADirectiveHandler {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif
