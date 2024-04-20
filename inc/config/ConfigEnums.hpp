#ifndef CONFIG_ENUMS_HPP
#define CONFIG_ENUMS_HPP

enum Context { DEFAULT, HTTP, SERVER, LOCATION };
enum Delimiter {
  SPACE = ' ',
  LEFT_BRACE = '{',
  RIGHT_BRACE = '}',
  SEMICOLON = ';',
  STRING = '*'
};

#endif
