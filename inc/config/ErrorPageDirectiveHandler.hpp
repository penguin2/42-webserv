#ifndef ERROR_PAGE_DIRECTIVE_HANDLER_HPP
#define ERROR_PAGE_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class ErrorPageDirectiveHandler : public ADirectiveHandler {
 public:
  ErrorPageDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();

  static std::vector<std::string> tokensToUniqueCodeStrings(
      const std::vector<std::string>& tokens);
  static std::vector<size_t> codeStringsToStatusCodes(
      const std::vector<std::string>& code_stirngs);
};

#endif
