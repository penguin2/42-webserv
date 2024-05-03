#include "./config/LocationConfig.hpp"

LocationConfig::LocationConfig() {}

const std::string& LocationConfig::getRoot() const { return root; }

void LocationConfig::setLocationRoot(const std::string& newRoot) {
  root = newRoot;
}

const std::string& LocationConfig::getIndex() const { return index; }
void LocationConfig::setIndex(const std::string& newIndex) { index = newIndex; }

const std::string& LocationConfig::getTryFilesPath() const {
  return try_files_path;
}
void LocationConfig::setTryFilesPath(const std::string& newPath) {
  try_files_path = newPath;
}

int LocationConfig::getTryFilesErrorCode() const {
  return try_files_error_code;
}
void LocationConfig::setTryFilesErrorCode(int newErrorCode) {
  try_files_error_code = newErrorCode;
}

const std::string& LocationConfig::getClientMaxBodySize() const {
  return client_max_body_size;
}
void LocationConfig::setClientMaxBodySize(const std::string& newSize) {
  client_max_body_size = newSize;
}

bool LocationConfig::getAutoindex() const { return autoindex; }
void LocationConfig::setAutoindex(bool newAutoindex) {
  autoindex = newAutoindex;
}

const std::vector<HttpMethod>& LocationConfig::getAllowMethods() const {
  return allow_methods;
}
void LocationConfig::setAllowMethods(
    const std::vector<HttpMethod> new_methods) {
  this->allow_methods = new_methods;
}

int LocationConfig::getReturnStatusCode() const {
  return return_status_code;
}
void LocationConfig::setReturnStatusCode(int newStatusCode) {
  return_status_code = newStatusCode;
}

const std::string& LocationConfig::getReturnUri() const { return return_uri; }
void LocationConfig::setReturnUri(const std::string& newUri) {
  return_uri = newUri;
}

const std::vector<std::string>& LocationConfig::getCgiPath() const {
  return cgi_path;
}
void LocationConfig::addCgiPath(const std::string& newPath) {
  cgi_path.push_back(newPath);
}

const std::vector<std::string>& LocationConfig::getCgiExt() const {
  return cgi_ext;
}
void LocationConfig::addCgiExt(const std::string& newExt) {
  cgi_ext.push_back(newExt);
}

void LocationConfig::print() {
  std::cout << "index: " << this->index << std::endl;
  std::cout << "autoindex: " << this->autoindex << std::endl;
  std::cout << "return status_code: " << this->return_status_code << std::endl;
  std::cout << "return uri: " << this->return_uri << std::endl;

  for (size_t i = 0; i < this->cgi_path.size(); i++) {
    std::cout << "cgi_path: " << this->cgi_path[i] << std::endl;
  }
  for (size_t i = 0; i < this->cgi_ext.size(); i++) {
    std::cout << "cgi_ext: " << this->cgi_ext[i] << std::endl;
  }
  std::cout << "client_max_body_size: " << this->client_max_body_size
            << std::endl;
  std::cout << "root: " << this->root << std::endl;
  std::cout << "allow_methods: ";

  for (size_t i = 0; i < allow_methods.size(); i++) {
    switch (allow_methods[i]) {
      case GET:
        std::cout << "GET ";
        break;
      case POST:
        std::cout << "POST ";
        break;
      case DELETE:
        std::cout << "DELETE ";
        break;
    }
  }
  std::cout << std::endl;
}
