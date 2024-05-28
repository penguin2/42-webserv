#include "Cookie.hpp"

#include "HttpUtils.hpp"
#include "Utils.hpp"

Cookie::Cookie(void) : secure_(false), httponly_(false) {}
Cookie::~Cookie(void) {}

Cookie::Cookie(const Cookie& cookie)
    : name_(cookie.name_),
      value_(cookie.value_),
      expires_(cookie.expires_),
      max_age_(cookie.max_age_),
      domain_(cookie.domain_),
      path_(cookie.path_),
      secure_(cookie.secure_),
      httponly_(cookie.httponly_) {}

const Cookie& Cookie::operator=(const Cookie& cookie) {
  if (this != &cookie) {
    this->name_ = cookie.name_;
    this->value_ = cookie.value_;
    this->expires_ = cookie.expires_;
    this->max_age_ = cookie.max_age_;
    this->domain_ = cookie.domain_;
    this->path_ = cookie.path_;
    this->secure_ = cookie.secure_;
    this->httponly_ = cookie.httponly_;
  }
  return *this;
}

void Cookie::setName(const std::string& cookie_name) {
  this->name_ = cookie_name;
}

void Cookie::setValue(const std::string& cookie_value) {
  this->value_ = cookie_value;
}

bool Cookie::setAndCheckExpires(const std::string& expires) {
  this->expires_ = expires;
  return true;
}

bool Cookie::setAndCheckMaxAge(const std::string& max_age) {
  this->max_age_ = max_age;
  return true;
}

bool Cookie::setAndCheckDomain(const std::string& domain) {
  this->domain_ = domain;
  return true;
}

bool Cookie::setAndCheckPath(const std::string& path) {
  this->path_ = path;
  return true;
}

bool Cookie::setSecure(bool is_secure) {
  this->secure_ = is_secure;
  return true;
}

bool Cookie::setHttpOnly(bool is_http_only) {
  this->httponly_ = is_http_only;
  return true;
}

const std::string& Cookie::getName(void) const { return name_; }
const std::string& Cookie::getValue(void) const { return value_; }
const std::string& Cookie::getExpires(void) const { return expires_; }
const std::string& Cookie::getMaxAge(void) const { return max_age_; }
const std::string& Cookie::getDomain(void) const { return domain_; }
const std::string& Cookie::getPath(void) const { return path_; }
bool Cookie::isSecure(void) const { return secure_; }
bool Cookie::isHttpOnly(void) const { return httponly_; }
