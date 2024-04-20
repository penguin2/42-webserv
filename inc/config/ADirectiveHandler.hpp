#ifndef ADIRECTIVE_HANDLER_HPP
#define ADIRECTIVE_HANDLER_HPP

#include <string>
#include <vector>

#include "ConfigEnums.hpp"

class ADirectiveHandler {
 protected:
  Context directive_context_;

 public:
  // ADirectiveHandler(Context context)
  //     : directive_context_(context) {}
  virtual bool isValid(const std::vector<std::string>& tokens) const = 0;
  // bool isMatchContext(Context context) {}
};

#endif
