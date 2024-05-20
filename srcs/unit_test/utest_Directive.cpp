#include <gtest/gtest.h>

#include "config/AllowMethodsDirectiveHandler.hpp"
#include "config/AutoIndexDirectiveHandler.hpp"
#include "config/CgiExtDirectiveHandler.hpp"
#include "config/ConfigParser.hpp"
#include "config/ServerNameDirectiveHandler.hpp"

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
  if (directive_handler.isSyntaxValid()) {
    EXPECT_EQ(directive_handler.isDirectiveValid(), expected)
        << "testing... " << line;
  }
}

TEST(Directive, AllowMethodsDirectiveHandler) {
  typedef AllowMethodsDirectiveHandler DirectiveHandler;

  // true
  testIsValid<DirectiveHandler>("allow_methods GET;", true);
  testIsValid<DirectiveHandler>("allow_methods POST;", true);
  testIsValid<DirectiveHandler>("allow_methods DELETE;", true);
  testIsValid<DirectiveHandler>("allow_methods DELETE GET;", true);
  testIsValid<DirectiveHandler>("allow_methods GET POST DELETE;", true);

  // false
  testIsValid<DirectiveHandler>("", false);
  testIsValid<DirectiveHandler>("allow_methods get;", false);
  testIsValid<DirectiveHandler>("allow_methods post;", false);
  testIsValid<DirectiveHandler>("allow_methods delete;", false);
  testIsValid<DirectiveHandler>("hello world;", false);

  testIsValid<DirectiveHandler>("allow_methods head;", false);
  testIsValid<DirectiveHandler>("allow_methods HEAD;", false);
  testIsValid<DirectiveHandler>("allow_methods get POST DELETE;", false);

  testIsValid<DirectiveHandler>("allow_methods GET GET POST DELETE;", false);

  testIsValid<DirectiveHandler>("allow_methods GET; POST DELETE;", false);
  testIsValid<DirectiveHandler>("allow_methods;", false);
}

TEST(Directive, AutoIndexDirectiveHandler) {
  typedef AutoIndexDirectiveHandler DirectiveHandler;

  testIsValid<DirectiveHandler>("", true);
}

TEST(Directive, CgiExtDirectiveHandler) {
  typedef CgiExtDirectiveHandler DirectiveHandler;

  testIsValid<DirectiveHandler>("", true);
}

TEST(Directive, serverNameDirectiveHandler) {
  typedef ServerNameDirectiveHandler DirectiveHandler;

  testIsValid<DirectiveHandler>("server_name 1;", true);
  testIsValid<DirectiveHandler>("server_name localhost;", true);
  testIsValid<DirectiveHandler>("server_name 0.0.0.0;", true);
  testIsValid<DirectiveHandler>("server_name 100.115.92.194;", true);
  testIsValid<DirectiveHandler>("server_name '';", true);
  testIsValid<DirectiveHandler>("server_name a_b-c~d;", true);
  testIsValid<DirectiveHandler>("server_name !a$b&c(d)*e+f,=;", true);

  testIsValid<DirectiveHandler>("server_name ", false);
  testIsValid<DirectiveHandler>("server_name ;", false);
  testIsValid<DirectiveHandler>("server_name localhost 8080;", false);
  testIsValid<DirectiveHandler>("server_name google.com/abc;", false);
  testIsValid<DirectiveHandler>("server_name google.%20com;", false);
  testIsValid<DirectiveHandler>("server_name google.com#abc;", false);
  testIsValid<DirectiveHandler>("server_name localhost:80;", false);
  testIsValid<DirectiveHandler>("server_name youser:pass@localhost;", false);
  testIsValid<DirectiveHandler>("server_name http://google.com;", false);
  testIsValid<DirectiveHandler>("server_name \"\";", false);
}

// ...
//
// TEST(Directive, 'SomeDirectiveHandler') {
//   typedef 'SomeDirectiveHandler' DirectiveHandler;
//
//   testIsValid<DirectiveHandler>('line_to_test', 'true if "line_to_test" is
//   valid else false');
// }
