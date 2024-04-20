#ifndef ADIRECTIVE_HANDLER_HPP
#define ADIRECTIVE_HANDLER_HPP

#include <string>
#include <vector>

#include "ConfigEnums.hpp"

class ADirectiveHandler {
 private:
  // Context directive_context_;

 public:
  // ADirectiveHandler::ADirectiveHandler(Context context)
  //     : directive_context(context) {}
  virtual bool isValid(const std::vector<std::string>& tokens) const = 0;
  // bool isMatchContext(Context context) {}
};

#endif
