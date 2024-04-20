#ifndef LISTEN_DIRECTIVE_HANDLER_HPP
#define LISTEN_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ListenDirectiveHandler : public ADirectiveHandler {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif
