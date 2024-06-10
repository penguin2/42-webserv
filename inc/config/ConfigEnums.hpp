#ifndef CONFIG_ENUMS_HPP
#define CONFIG_ENUMS_HPP

namespace ConfigEnums {
  enum Context { DEFAULT, HTTP, SERVER, LOCATION };
  enum Delimiter {
    SPACE = ' ',
    TAB = '\t',
    LEFT_BRACE = '{',
    RIGHT_BRACE = '}',
    SEMICOLON = ';',
    STRING = '*'
  };
}

#endif
