#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
  this->current_context = DEFAULT;
  this->current_delimiter = SPACE;
}

void ConfigParser::parseConfig(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    parseLine(line);
  }
}

void ConfigParser::parseLine(const std::string& line) {
  if (line.empty() || line[0] == '#') {
    return;
  }

  std::vector<std::string> tokens;
  tokenize(line, tokens);
  if (tokens.size() == 0) {
    std::cerr << "syntax error : line with only space detected";
    exit(1);
  }

  if (tokens[0] == "http") {
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
    handleDirective(tokens);
  }
}

void ConfigParser::handleDirective(const std::vector<std::string>& tokens) {
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

void ConfigParser::tokenize(const std::string& line,
                            std::vector<std::string>& tokens) {
  std::istringstream iss(line);
  std::string token;
  char c;
  while (iss.get(c)) {
    switch (c) {
      case static_cast<char>(SPACE):
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
        break;
      case static_cast<char>(LEFT_BRACE):
      case static_cast<char>(RIGHT_BRACE):
      case static_cast<char>(SEMICOLON):
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
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.conf>" << std::endl;
        return 1;
    }
    std::string configFileName = argv[1];

    ConfigParser parser;
    parser.parseConfig(configFileName);

    return 0;
}
