#include <gtest/gtest.h>

#include "config/AllowMethodsDirectiveHandler.hpp"
#include "config/AutoIndexDirectiveHandler.hpp"
#include "config/CgiExtDirectiveHandler.hpp"
#include "config/ConfigParser.hpp"
#include "config/ListenDirectiveHandler.hpp"

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

TEST(Directive, ListenDirectiveHandler) {
  typedef ListenDirectiveHandler DirectiveHandler;

  testIsValid<DirectiveHandler>("listen 0000000001;", true);
  testIsValid<DirectiveHandler>("listen 65535;", true);
  testIsValid<DirectiveHandler>("listen 255.255.255.255:80;", true);
  testIsValid<DirectiveHandler>("listen 0.0.0.0:443;", true);
  testIsValid<DirectiveHandler>("listen 127.0.0.1:25;", true);
  testIsValid<DirectiveHandler>("listen LoCaLhOsT:1024;", true);

  testIsValid<DirectiveHandler>("listen ;", false);
  testIsValid<DirectiveHandler>("listen  ", false);
  testIsValid<DirectiveHandler>("listen  0.0.0.0 80;", false);
  testIsValid<DirectiveHandler>("listen  0.0.0.0.:80;", false);
  testIsValid<DirectiveHandler>("listen  .0.0.0.0:80;", false);
  testIsValid<DirectiveHandler>("listen  .0.0.0.0.:80;", false);
  testIsValid<DirectiveHandler>("listen  0..0...0.0:80;", false);
  testIsValid<DirectiveHandler>("listen  ...:80;", false);
  testIsValid<DirectiveHandler>("listen  .0.0.0:80;", false);
  testIsValid<DirectiveHandler>("listen  0.0.0.:80;", false);
  testIsValid<DirectiveHandler>("listen  -1.0.0.0:80;", false);
  testIsValid<DirectiveHandler>("listen  .0.0.:80;", false);
  testIsValid<DirectiveHandler>("listen  127.0.0.0:80 ab;", false);
  testIsValid<DirectiveHandler>("listen a;", false);
  testIsValid<DirectiveHandler>("listen -1;", false);
  testIsValid<DirectiveHandler>("listen 0;", false);
  testIsValid<DirectiveHandler>("listen 0000000000;", false);
  testIsValid<DirectiveHandler>("listen 65536;", false);
  testIsValid<DirectiveHandler>("listen :65535;", false);
  testIsValid<DirectiveHandler>("listen 0.0.0.0:;", false);
  testIsValid<DirectiveHandler>("listen 256.0.0.1:80;", false);
  testIsValid<DirectiveHandler>("listen 0000001.0.0.1:80;", false);
  testIsValid<DirectiveHandler>("listen a.b.c.d:80;", false);
}

// ...
//
// TEST(Directive, 'SomeDirectiveHandler') {
//   typedef 'SomeDirectiveHandler' DirectiveHandler;
//
//   testIsValid<DirectiveHandler>('line_to_test', 'true if "line_to_test" is
//   valid else false');
// }
