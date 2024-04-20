#ifndef ROOT_DIRECTIVE_HANDLER_HPP
#define ROOT_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class RootDirectiveHandler : public ADirectiveHandler {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif
