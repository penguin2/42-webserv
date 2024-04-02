#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class NginxConfigParser {
 public:
  NginxConfigParser() {
    current_context = DEFAULT;
    current_delimiter = SPACE;
  }
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
  enum Context { DEFAULT, HTTP, SERVER, LOCATION };
  Context current_context;
  enum Delimiter {
    SPACE = ' ',
    LEFT_BRACE = '{',
    RIGHT_BRACE = '}',
    SEMICOLON = ';',
    STRING = '*'
  };

  Delimiter current_delimiter;

  void ParseLine(const std::string& line) {
    if (line.empty() || line[0] == '#') {
      return;
    }

    std::vector<std::string> tokens;
    Tokenize(line, tokens);

    if (tokens[0] == "events") {
      current_context = DEFAULT;
    } else if (tokens[0] == "http") {
      current_context = HTTP;
    } else if (tokens[0] == "server") {
      current_context = SERVER;
    } else if (tokens[0] == "location") {
      current_context = LOCATION;
    } else if (tokens[0] == "}") {
      if (current_context != DEFAULT) {
        current_context =
            static_cast<Context>(static_cast<int>(current_context) - 1);
      } else {
        std::cerr << "syntax error : Unexpected '}' outside any block"
                  << std::endl;
        exit(1);
      }
    } else {
      HandleDirective(tokens);
    }
  }

  void HandleDirective(const std::vector<std::string>& tokens) {
    // TODO contextに合っていない要素についてはすべて文法エラーとする
    switch (current_context) {
      case HTTP:
        std::cout << "Inside HTTP block: " << tokens[0] << std::endl;
        break;
      case SERVER:
        std::cout << "Inside server block: " << tokens[0] << std::endl;
        break;
      case LOCATION:
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
    // TODO delimiterの前がおかしいものはすべて文法エラーとする
    while (iss.get(c)) {
      switch (c) {
        case static_cast<char>(SPACE):
          this->current_delimiter = SPACE;
          break;
        case static_cast<char>(LEFT_BRACE):
          this->current_delimiter = LEFT_BRACE;
          break;
        case static_cast<char>(RIGHT_BRACE):
          this->current_delimiter = RIGHT_BRACE;
          break;
        case static_cast<char>(SEMICOLON):
          this->current_delimiter = SEMICOLON;
          break;
        default:
          this->current_delimiter = STRING;
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
  parser.ParseConfig("sample.conf");
  return 0;
}
