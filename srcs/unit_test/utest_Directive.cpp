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

  testIsValid<DirectiveHandler>("cgi_ext .py;", true);
  testIsValid<DirectiveHandler>("cgi_ext .php;", true);
  testIsValid<DirectiveHandler>("cgi_ext .php .php;", true);
  testIsValid<DirectiveHandler>("cgi_ext .c;", true);
  testIsValid<DirectiveHandler>("cgi_ext .abcdefg;", true);

#if defined(BONUS)
  testIsValid<DirectiveHandler>("cgi_ext .py .php .c;", true);
  testIsValid<DirectiveHandler>("cgi_ext .php .py .py .php;", true);
  testIsValid<DirectiveHandler>("cgi_ext .php .py .py .;", false);
#endif

  testIsValid<DirectiveHandler>("cgi_ext ", false);
  testIsValid<DirectiveHandler>("cgi_ext ;", false);
  testIsValid<DirectiveHandler>("cgi_ext .;", false);
  testIsValid<DirectiveHandler>("cgi_ext test.py;", false);
  testIsValid<DirectiveHandler>("cgi_ext py.;", false);
  testIsValid<DirectiveHandler>("cgi_ext .a.;", false);
  testIsValid<DirectiveHandler>("cgi_ext ..;", false);
  testIsValid<DirectiveHandler>("cgi_ext ..py;", false);
  testIsValid<DirectiveHandler>("cgi_ext .tar.gz;", false);
  testIsValid<DirectiveHandler>("cgi_ext ls;", false);
  testIsValid<DirectiveHandler>("cgi_ext ./ls;", false);
  testIsValid<DirectiveHandler>("cgi_ext .py/abc;", false);
  testIsValid<DirectiveHandler>("cgi_ext .php/;", false);
}

// ...
//
// TEST(Directive, 'SomeDirectiveHandler') {
//   typedef 'SomeDirectiveHandler' DirectiveHandler;
//
//   testIsValid<DirectiveHandler>('line_to_test', 'true if "line_to_test" is
//   valid else false');
// }
