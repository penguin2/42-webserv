#include "./config/LocationConfig.hpp"

LocationConfig::LocationConfig() {}

const std::string& LocationConfig::getRoot() const {
  return root;
}

const std::string& LocationConfig::getIndex() const {
  return index;
}

const std::string& LocationConfig::getTryFilesPath() const {
  return try_files_path;
}

int LocationConfig::getTryFilesErrorCode() const {
  return try_files_error_code;
}

const std::string& LocationConfig::getClientMaxBodySize() const {
  return client_max_body_size;
}

bool LocationConfig::getAutoindex() const {
  return autoindex;
}

const std::vector<HttpMethod>& LocationConfig::getLimitExceptMethods() const {
  return allow_methods;
}

const std::string& LocationConfig::getReturnStatusCode() const {
  return return_status_code;
}

const std::string& LocationConfig::getReturnUri() const {
  return return_uri;
}

const std::string& LocationConfig::getCgiPath() const {
  return cgi_path;
}

const std::string& LocationConfig::getCgiExt() const {
  return cgi_ext;
}

void LocationConfig::print() { 
  std::cout << "autoindex: " << this->autoindex << std::endl;
  std::cout << "cgi_ext: " << this->cgi_ext << std::endl;
  std::cout << "cgi_path: " << this->cgi_path << std::endl;
  std::cout << "client_max_body_size: " << this->client_max_body_size << std::endl;
}
