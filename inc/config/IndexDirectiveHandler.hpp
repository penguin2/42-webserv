#ifndef INDEX_DIRECTIVE_HANDLER_HPP
#define INDEX_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class IndexDirectiveHandler : public ADirectiveHandler {
 public:
  virtual bool isValid(const std::vector<std::string>& tokens) const;
};

#endif
