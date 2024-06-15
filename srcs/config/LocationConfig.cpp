#include "./config/LocationConfig.hpp"

LocationConfig::LocationConfig()
    : client_max_body_size(0),
      autoindex(false),
      allow_upload(false),
      return_status_code(0) {}

const std::string& LocationConfig::getRoot() const { return root; }

void LocationConfig::setRoot(const std::string& newRoot) { root = newRoot; }

const std::string& LocationConfig::getIndex() const { return index; }
void LocationConfig::setIndex(const std::string& newIndex) { index = newIndex; }

size_t LocationConfig::getClientMaxBodySize() const {
  return client_max_body_size;
}
void LocationConfig::setClientMaxBodySize(size_t newSize) {
  client_max_body_size = newSize;
}

bool LocationConfig::getAutoindex() const { return autoindex; }
void LocationConfig::setAutoindex(bool newAutoindex) {
  autoindex = newAutoindex;
}

bool LocationConfig::getAllowUpload() const { return allow_upload; }
void LocationConfig::setAllowUpload(bool new_allow_upload) {
  this->allow_upload = new_allow_upload;
}

const std::vector<std::string>& LocationConfig::getAllowMethods() const {
  return allow_methods;
}
void LocationConfig::setAllowMethods(
    const std::vector<std::string> new_methods) {
  this->allow_methods = new_methods;
}

size_t LocationConfig::getReturnStatusCode() const {
  return return_status_code;
}

void LocationConfig::setReturnStatusCode(size_t newStatusCode) {
  return_status_code = newStatusCode;
}

const std::string& LocationConfig::getReturnUri() const { return return_uri; }
void LocationConfig::setReturnUri(const std::string& newUri) {
  return_uri = newUri;
}

const std::vector<std::string>& LocationConfig::getCgiExt() const {
  return cgi_ext;
}

void LocationConfig::setCgiExts(const std::vector<std::string> new_cgi_ext) {
  this->cgi_ext = new_cgi_ext;
}

void LocationConfig::addCgiExt(const std::string& newExt) {
  cgi_ext.push_back(newExt);
}

void LocationConfig::print() {
  std::cout << "index: " << this->index << std::endl;
  std::cout << "autoindex: " << this->autoindex << std::endl;

  std::cout << "return status_code: " << this->return_status_code << std::endl;
  std::cout << "return uri: " << this->return_uri << std::endl;

  for (size_t i = 0; i < this->cgi_ext.size(); i++) {
    std::cout << "cgi_ext: " << this->cgi_ext[i] << std::endl;
  }
  std::cout << "client_max_body_size: " << this->client_max_body_size
            << std::endl;
  std::cout << "root: " << this->root << std::endl;
  std::cout << "allow_methods: ";

  for (size_t i = 0; i < allow_methods.size(); i++) {
    std::cout << allow_methods[i];
  }
  std::cout << std::endl;
}
