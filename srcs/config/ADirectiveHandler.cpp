#include "./config/ADirectiveHandler.hpp"

ADirectiveHandler::ADirectiveHandler(){}

ADirectiveHandler::~ADirectiveHandler(){}

bool ADirectiveHandler::isMatchContext(Context context) {
  return directive_context_ == context;
}

void ADirectiveHandler::setToken(const std::vector<std::string>& tokens) {
  this->tokens = tokens;
}
