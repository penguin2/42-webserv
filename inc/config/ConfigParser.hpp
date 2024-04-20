#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib> 
#include <map>
#include "ADirectiveHandler.hpp"
#include "ListenDirectiveHandler.hpp"
#include "ServerNameDirectiveHandler.hpp"
#include "ErrorPageDirectiveHandler.hpp"
#include "AllowMethodsDirectiveHandler.hpp"
#include "RootDirectiveHandler.hpp"
#include "IndexDirectiveHandler.hpp"
#include "AutoIndexDirectiveHandler.hpp"
#include "TryFilesDirectiveHandler.hpp"
#include "ConfigEnums.hpp"

class ConfigParser {
 public:
  ConfigParser();

  void parseConfig(const std::string& filename);

 private:
  size_t http_count_;
  size_t server_count_;
  size_t location_count_;
  Context current_context_;
  Delimiter current_delimiter_;

  void parseLine(const std::string& line);

  void handleDirective(const std::vector<std::string>& tokens);

  void tokenize(const std::string& line, std::vector<std::string>& tokens);

  bool isValidPath(const std::string& path);

  void handleError(const std::string& errorMessage);
  
  std::map<std::string, ADirectiveHandler*> handlers;
};

#endif
