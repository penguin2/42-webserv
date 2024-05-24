#ifndef CGI_EXT_DIRECTIVE_HANDLER_HPP
#define CGI_EXT_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class CgiExtDirectiveHandler : public ADirectiveHandler {
 public:
  CgiExtDirectiveHandler();
  virtual bool isDirectiveValid() const;
  virtual void setConfig();
  static std::vector<std::string> tokensToUniqueCgiExts(
      const std::vector<std::string>& tokens);
};

#endif
