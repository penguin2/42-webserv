#include "./config/ADirectiveHandler.hpp"

ADirectiveHandler::ADirectiveHandler(){}

bool ADirectiveHandler::isMatchContext(Context context) {
  return directive_context_ == context;
}
