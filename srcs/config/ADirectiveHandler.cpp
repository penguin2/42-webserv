#include "./config/ADirectiveHandler.hpp"

ADirectiveHandler::ADirectiveHandler(){}

ADirectiveHandler::~ADirectiveHandler(){}

bool ADirectiveHandler::isMatchContext(ConfigEnums::Context context) {
  return directive_context_ == context;
}

ADirectiveHandler::setToken(const std::vector<std::string>& tokens) {
  this->tokens = tokens;
}
