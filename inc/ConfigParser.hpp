#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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

  void ParseConfig(const std::string& filename);

 private:
  Context current_context;
  Delimiter current_delimiter;

  void ParseLine(const std::string& line);

  void HandleDirective(const std::vector<std::string>& tokens);

  void Tokenize(const std::string& line, std::vector<std::string>& tokens);
};

#endif
