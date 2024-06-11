#include "./config/ConfigParser.hpp"

ConfigParser::ConfigParser()
    : http_count_(0),
      server_count_(0),
      location_count_(0),
      current_context_(ConfigEnums::DEFAULT),
      current_delimiter_(ConfigEnums::SPACE) {
  this->handlers["listen"] = new ListenDirectiveHandler();
  this->handlers["server_name"] = new ServerNameDirectiveHandler();
  this->handlers["error_page"] = new ErrorPageDirectiveHandler();
  this->handlers["allow_methods"] = new AllowMethodsDirectiveHandler();
  this->handlers["root"] = new RootDirectiveHandler();
  this->handlers["index"] = new IndexDirectiveHandler();
  this->handlers["autoindex"] = new AutoIndexDirectiveHandler();
  this->handlers["upload"] = new UploadDirectiveHandler();
  this->handlers["try_files"] = new TryFilesDirectiveHandler();
  this->handlers["return"] = new ReturnDirectiveHandler();
  this->handlers["client_max_body_size"] =
      new ClientMaxBodySizeDirectiveHandler();
  this->handlers["cgi_ext"] = new CgiExtDirectiveHandler();
}

ConfigParser::~ConfigParser() {
  for (std::map<std::string, ADirectiveHandler*>::iterator it =
           handlers.begin();
       it != handlers.end(); ++it) {
    delete it->second;
  }
  handlers.clear();
}

void ConfigParser::parseConfig(const std::string& filename) {
  std::ifstream file(filename.c_str());
  if (!file.is_open()) {
    handleError("Failed to open file: " + filename);
  }

  std::string line;
  while (std::getline(file, line)) {
    parseLine(line);
  }

  if (current_context_ != ConfigEnums::DEFAULT) {
    handleError("syntax error : '}' is not enough");
  }

  if (http_count_ != 1) {
    handleError("syntax error: HTTP block must appear exactly once");
  }
}

void ConfigParser::parseLine(const std::string& line) {
  if (line.empty()) {
    return;
  }

  std::vector<std::string> tokens;
  tokenize(line, tokens);
  if (tokens.size() == 0) {
    handleError("syntax error : line with only space detected");
  }

  if (tokens[0] == "#") {
    return;
  }

  std::string last_tokens = tokens.back();

  if (tokens[0] == "http" && last_tokens == "{") {
    http_count_++;
    if (current_context_ != ConfigEnums::DEFAULT) {
      handleError("syntax error : http context must be in default context");
    }
    if (tokens.size() != 2) {
      handleError("syntax error : http requires no arguments");
    }
    if (http_count_ > 1) {
      handleError("syntax error: HTTP block must appear exactly once");
    }
    current_context_ = ConfigEnums::HTTP;
  } else if (tokens[0] == "server" && last_tokens == "{") {
    server_count_++;
    if (current_context_ != ConfigEnums::HTTP) {
      handleError("syntax error : server context must be in http context");
    }
    if (tokens.size() != 2) {
      handleError("syntax error : server requires no arguments");
    }
    current_context_ = ConfigEnums::SERVER;
  } else if (tokens[0] == "location" && last_tokens == "{") {
    location_count_++;
    if (current_context_ != ConfigEnums::SERVER) {
      handleError("syntax error : location context must be in server context");
    }
    if (tokens.size() == 2) {
      handleError("syntax error : location must contain path");
    } else if (tokens.size() == 3 && !isValidPath(tokens[1])) {
      handleError("syntax error : location path is invalid");
    } else if (tokens.size() >= 4) {
      handleError("syntax error : location too many path");
    }

    this->current_location_path_ = tokens[1];

    current_context_ = ConfigEnums::LOCATION;
  } else if (tokens[0] == "}") {
    if (current_context_ == ConfigEnums::DEFAULT) {
      handleError("syntax error : Unexpected '}' outside any block");
    } else if (current_context_ == ConfigEnums::HTTP) {
      if (server_count_ == 0) {
        handleError("syntax error : http requires one or more server");
      }
      server_count_ = 0;
    } else if (current_context_ == ConfigEnums::SERVER) {
      if (location_count_ == 0) {
        handleError("syntax error : server requires one or more location");
      }
      location_count_ = 0;
    }
    this->current_location_path_ = "";
    current_context_ = static_cast<ConfigEnums::Context>(
        static_cast<int>(current_context_) - 1);
    if (tokens.size() != 1) {
      handleError("syntax error : Unexpected argument after '}'");
    }
  } else {
    handleDirective(tokens);
  }
}

void ConfigParser::handleDirective(const std::vector<std::string>& tokens) {
  if (this->handlers.find(tokens[0]) == this->handlers.end()) {
    handleError(tokens[0] + " does not exist");
  }

  if (!this->handlers[tokens[0]]->isMatchContext(current_context_)) {
    handleError(tokens[0] + " exists in invalid location");
  }

  this->handlers[tokens[0]]->setToken(tokens);
  if (!this->handlers[tokens[0]]->isSyntaxValid()) {
    handleError("syntax error : " + tokens[0] + " is invalid");
  }
  if (!this->handlers[tokens[0]]->isDirectiveValid()) {
    handleError("syntax error : " + tokens[0] + " is invalid");
  }

  this->handlers[tokens[0]]->setServerNum(this->server_count_);
  this->handlers[tokens[0]]->setLocationPath(this->current_location_path_);
  this->handlers[tokens[0]]->setConfig();
}

void ConfigParser::tokenize(const std::string& line,
                            std::vector<std::string>& tokens) {
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

void ConfigParser::handleError(const std::string& errorMessage) {
  std::cerr << errorMessage << std::endl;
  exit(1);
}
