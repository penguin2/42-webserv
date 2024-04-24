#ifndef ADIRECTIVE_HANDLER_HPP
#define ADIRECTIVE_HANDLER_HPP

#include <string>
#include <vector>
#include <iostream>

#include "ConfigEnums.hpp"
#include "Config.hpp"
#include "ServerConfig.hpp"

class ADirectiveHandler {
 protected:
  ConfigEnums::Context directive_context_;
  std::vector<std::string> tokens;

 public:
  ADirectiveHandler();
  virtual ~ADirectiveHandler();
  virtual bool isValid() const = 0;
  virtual bool isMatchContext(ConfigEnums::Context context);
  virtual void setToken(const std::vector<std::string>& tokens);
  virtual void setConfig(long unsigned int server_num ,std::string location_path) = 0;
};

#endif
