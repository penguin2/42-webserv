#include "ListenDirectiveValidator.hpp"

bool ListenDirectiveValidator::isValid(
    const std::vector<std::string>& tokens) const {
  if (tokens.size() < 3) {
    return false;
  }
  return true;
}