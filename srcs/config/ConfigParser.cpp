#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {
  this->current_context_ = DEFAULT;
  this->current_delimiter_ = SPACE;
  this->http_count_ = 0;
  this->server_count_ = 0;
  this->location_count_ = 0;
}

void ConfigParser::parseConfig(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(file, line)) {
    parseLine(line);
  }

  if (current_context_ != DEFAULT) {
    std::cerr << "syntax error : '}' is not enough" << std::endl;
    exit(1);
  }

  if (http_count_ != 1) {
    std::cerr << "syntax error: HTTP block must appear exactly once"
              << std::endl;
    exit(1);
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

  std::string last_tokens = tokens.back();

  if (tokens[0] == "http" && last_tokens == "{") {
    http_count_++;
    if (current_context_ != DEFAULT) {
      std::cerr << "syntax error : http context must be in default context";
      exit(1);
    }
    if (tokens.size() != 2) {
      std::cerr << "syntax error : http requires no arguments.";
      exit(1);
    }
    if (http_count_ > 1) {
      std::cerr << "syntax error: HTTP block must appear exactly once"
                << std::endl;
      exit(1);
    }
    current_context_ = HTTP;
  } else if (tokens[0] == "server" && last_tokens == "{") {
    server_count_++;
    if (current_context_ != HTTP) {
      std::cerr << "syntax error : server context must be in http context";
      exit(1);
    }
    if (tokens.size() != 2) {
      std::cerr << "syntax error : server requires no arguments.";
      exit(1);
    }
    current_context_ = SERVER;
  } else if (tokens[0] == "location" && last_tokens == "{") {
    location_count_++;
    if (current_context_ != SERVER) {
      std::cerr << "syntax error : location context must be in server context";
      exit(1);
    }
    if (tokens.size() == 2) {
      std::cerr << "syntax error : location must contain path";
      exit(1);
    } else if (tokens.size() == 3 && !isValidPath(tokens[1])) {
      std::cerr << "syntax error : location path is invalid";
      exit(1);
    } else if (tokens.size() >= 4) {
      std::cerr << "syntax error : location too many path";
      exit(1);
    }

    current_context_ = LOCATION;
  } else if (tokens[0] == "}") {
    if (current_context_ == DEFAULT) {
      std::cerr << "syntax error : Unexpected '}' outside any block"
                << std::endl;
      exit(1);
    } else if (current_context_ == HTTP) {
      if (server_count_ == 0) {
        std::cerr << "syntax error : http requires one or more server"
                  << std::endl;
        exit(1);
      }
      server_count_ = 0;
    } else if (current_context_ == SERVER) {
      if (location_count_ == 0) {
        std::cerr << "syntax error : server requires one or more location"
                  << std::endl;
        exit(1);
      }
      location_count_ = 0;
    }
    current_context_ =
        static_cast<Context>(static_cast<int>(current_context_) - 1);
    if (tokens.size() != 1) {
      std::cerr << "syntax error : Unexpected argument after '}'" << std::endl;
      exit(1);
    }
  } else {
    handleDirective(tokens);
  }
}

void ConfigParser::handleDirective(const std::vector<std::string>& tokens) {
  // TODO contextに合っていない要素についてはすべて文法エラーとする
  switch (current_context_) {
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

bool ConfigParser::isValidPath(const std::string& path) {
  if (path.empty()) {
    return false;
  }
  const std::string forbiddenChars = "\\:*?\"<>|";
  for (size_t i = 0; i < path.size(); i++) {
    if (forbiddenChars.find(path[i]) != std::string::npos) {
      return false;
    }
  }
  return true;
}

// int main(int argc, char* argv[]) {
//   if (argc != 2) {
//     std::cerr << "Usage: " << argv[0] << " <config_file.conf>" << std::endl;
//     return 1;
//   }
//   std::string configFileName = argv[1];

//   ConfigParser parser;
//   parser.parseConfig(configFileName);

//   return 0;
// }
