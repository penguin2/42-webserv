#ifndef ADIRECTIVE_VALIDATOR_HPP
#define ADIRECTIVE_VALIDATOR_HPP

#include <string>
#include <vector>

class ADirectiveValidator {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const = 0;
};

#endif
