#ifndef ADIRECTIVE_HANDLER_HPP
#define ADIRECTIVE_HANDLER_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Config.hpp"
#include "ConfigEnums.hpp"
#include "Logger.hpp"
#include "ServerConfig.hpp"
#include "utils/Utils.hpp"

class ADirectiveHandler {
 protected:
  ConfigEnums::Context directive_context_;
  std::vector<std::string> tokens_;
  int server_num_;
  std::string location_path_;

 public:
  ADirectiveHandler();
  virtual ~ADirectiveHandler();
  virtual bool isSyntaxValid() const;
  virtual bool isDirectiveValid() const = 0;
  virtual bool isMatchContext(ConfigEnums::Context context);
  virtual void setToken(const std::vector<std::string>& tokens);
  virtual void setServerNum(long unsigned int server_num);
  virtual void setLocationPath(std::string location_path);
  virtual void setConfig() = 0;
  virtual void log();
  virtual ServerConfig& getServerConfig();
  virtual LocationConfig& getLocationConfig();
};

#endif
