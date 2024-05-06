#ifndef ADIRECTIVE_HANDLER_HPP
#define ADIRECTIVE_HANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Config.hpp"
#include "ConfigEnums.hpp"
#include "Logger.hpp"
#include "ServerConfig.hpp"
#include "Utils.hpp"

class ADirectiveHandler {
 protected:
  ConfigEnums::Context directive_context_;
  std::vector<std::string> tokens_;
  int server_num_;
  std::string location_path_;

 public:
  ADirectiveHandler();
  virtual ~ADirectiveHandler();
  virtual bool isValid() const = 0;
  virtual bool isMatchContext(ConfigEnums::Context context);
  virtual void setToken(const std::vector<std::string>& tokens);
  virtual void setServerNum(long unsigned int server_num);
  virtual void setLocationPath(std::string location_path);
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path) = 0;
  virtual void log();
};

#endif
