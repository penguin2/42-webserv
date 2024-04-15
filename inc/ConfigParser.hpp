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

enum Context { DEFAULT, HTTP, SERVER, LOCATION };
enum Delimiter {
  SPACE = ' ',
  LEFT_BRACE = '{',
  RIGHT_BRACE = '}',
  SEMICOLON = ';',
  STRING = '*'
};

class ConfigParser {
 public:
  ConfigParser();

  void parseConfig(const std::string& filename);

 private:
  int http_count_;
  int server_count_;
  int location_count_;
  Context current_context_;
  Delimiter current_delimiter_;

  void parseLine(const std::string& line);

  void handleDirective(const std::vector<std::string>& tokens);

  void tokenize(const std::string& line, std::vector<std::string>& tokens);

  bool isValidPath(const std::string& path);
};

#endif
