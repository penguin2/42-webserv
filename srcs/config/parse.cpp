#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class NginxConfigParser {
 public:
  void ParseConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return;
    }

    std::string line;
    while (std::getline(file, line)) {
        ParseLine(line);
    }
  }

 private:
  enum class Context { DEFAULT, HTTP, SERVER, LOCATION };

  enum class Delimiter {
    SPACE = ' ',
    LEFT_BRACE = '{',
    RIGHT_BRACE = '}',
    SEMICOLON = ';'
  };

  Context current_context = Context::DEFAULT;
  Delimiter current_delimiter = Delimiter::SPACE;

  void ParseLine(const std::string& line) {
    if (line.empty() || line[0] == '#') {
      return;
    }

    std::vector<std::string> tokens;
    Tokenize(line, tokens);

    if (tokens[0] == "events") {
      current_context = Context::DEFAULT;
    } else if (tokens[0] == "http") {
      current_context = Context::HTTP;
    } else if (tokens[0] == "server") {
      current_context = Context::SERVER;
    } else if (tokens[0] == "location") {
      current_context = Context::LOCATION;
    } else if (tokens[0] == "}") {
      if (current_context != Context::DEFAULT) {
        current_context =
            static_cast<Context>(static_cast<int>(current_context) - 1);
      } else {
        throw std::invalid_argument("Unexpected '}' outside any block");
      }
    } else {
      HandleDirective(tokens);
    }
  }

  void HandleDirective(const std::vector<std::string>& tokens) {
    switch (current_context) {
      case Context::HTTP:
        std::cout << "Inside HTTP block: " << tokens[0] << std::endl;
        break;
      case Context::SERVER:
        std::cout << "Inside server block: " << tokens[0] << std::endl;
        break;
      case Context::LOCATION:
        std::cout << "Inside location block: " << tokens[0] << std::endl;
        break;
      default:
        std::cerr << "Directive outside any block: " << tokens[0] << std::endl;
        break;
    }
  }

  void Tokenize(const std::string& line, std::vector<std::string>& tokens) {
    std::istringstream iss(line);
    std::string token;
    char c;
    while (iss.get(c)) {
      Delimiter delimiter;
      switch (c) {
        case static_cast<char>(Delimiter::SPACE):
          this->current_delimiter = Delimiter::SPACE;
          break;
        case static_cast<char>(Delimiter::LEFT_BRACE):
          this->current_delimiter = Delimiter::LEFT_BRACE;
          break;
        case static_cast<char>(Delimiter::RIGHT_BRACE):
          this->current_delimiter = Delimiter::RIGHT_BRACE;
          break;
        case static_cast<char>(Delimiter::SEMICOLON):
          this->current_delimiter = Delimiter::SEMICOLON;
          break;
        default:
          delimiter = Delimiter::SPACE;
          break;
      }

      if (!token.empty()) {
        tokens.push_back(token);
      }

      token = c;
      tokens.push_back(token);
      token.clear();
    }
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }
};

int main() {
  NginxConfigParser parser;
  parser.ParseConfig("nginx.conf");
  return 0;
}
