#ifndef ADIRECTIVE_HANDLER_HPP
#define ADIRECTIVE_HANDLER_HPP

#include <string>
#include <vector>

#include "ConfigEnums.hpp"

class ADirectiveHandler {
 protected:
  Context directive_context_;

 public:
  ADirectiveHandler();
  virtual bool isValid(const std::vector<std::string>& tokens) const = 0;
  virtual bool isMatchContext(Context context);
};

#endif
