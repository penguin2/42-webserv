#include <gtest/gtest.h>

#include "config/AllowMethodsDirectiveHandler.hpp"
#include "config/AutoIndexDirectiveHandler.hpp"
#include "config/CgiExtDirectiveHandler.hpp"
#include "config/ConfigParser.hpp"

static std::vector<std::string> tempTokenize(const std::string& line) {
  std::vector<std::string> tokens;

  std::istringstream iss(line);
  std::string token;
  char c;
  while (iss.get(c)) {
    switch (c) {
      case static_cast<char>(ConfigEnums::SPACE):
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
        break;
      case static_cast<char>(ConfigEnums::LEFT_BRACE):
      case static_cast<char>(ConfigEnums::RIGHT_BRACE):
      case static_cast<char>(ConfigEnums::SEMICOLON):
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
        token = c;
        tokens.push_back(token);
        token.clear();
        break;
      default:
        token += c;
        break;
    }
  }
  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}

template <typename T>
static void testIsValid(const std::string& line, bool expected) {
  T directive_handler;
  directive_handler.setToken(tempTokenize(line));
  EXPECT_EQ(directive_handler.isValid(), expected);
}

TEST(Directive, AllowMethodsDirectiveHandler) {
  typedef AllowMethodsDirectiveHandler DirectiveHandler;

  //true
  testIsValid<DirectiveHandler>("", true);
  testIsValid<DirectiveHandler>("hello world", true);
  testIsValid<DirectiveHandler>("allow_methods get", true);
  testIsValid<DirectiveHandler>("allow_methods GET", true);
  testIsValid<DirectiveHandler>("allow_methods post", true);
  testIsValid<DirectiveHandler>("allow_methods POST", true);
  testIsValid<DirectiveHandler>("allow_methods delete", true);
  testIsValid<DirectiveHandler>("allow_methods DELETE", true);

  //false
  testIsValid<DirectiveHandler>("allow_methods head", true);
  testIsValid<DirectiveHandler>("allow_methods HEAD", true);
  testIsValid<DirectiveHandler>("allow_methods GET POST DELETE", true);
  testIsValid<DirectiveHandler>("allow_methods get POST DELETE", true);
}

TEST(Directive, AutoIndexDirectiveHandler) {
  typedef AutoIndexDirectiveHandler DirectiveHandler;

  testIsValid<DirectiveHandler>("", true);
}

TEST(Directive, CgiExtDirectiveHandler) {
  typedef CgiExtDirectiveHandler DirectiveHandler;

  testIsValid<DirectiveHandler>("", true);
}

// ...
//
// TEST(Directive, 'SomeDirectiveHandler') {
//   typedef 'SomeDirectiveHandler' DirectiveHandler;
//
//   testIsValid<DirectiveHandler>('line_to_test', 'true if "line_to_test" is valid else false');
// }
