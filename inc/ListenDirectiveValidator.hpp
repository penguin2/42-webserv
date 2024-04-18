#ifndef LISTEN_DIRECTIVE_VALIDATOR_HPP
#define LISTEN_DIRECTIVE_VALIDATOR_HPP

#include "ADirectiveValidator.hpp"

class ListenDirectiveValidator : public ADirectiveValidator {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif