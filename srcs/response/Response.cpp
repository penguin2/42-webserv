#include "Response.hpp"

#include <ctime>

#include "Cookie.hpp"
#include "CookieManager.hpp"
#include "HttpUtils.hpp"
#include "ResponseData.hpp"

Response::Response(void) { this->data_ = new ResponseData; }
Response::~Response(void) { delete this->data_; }

void Response::getResponseRawData(std::stringstream &ss) const {
  const std::map<std::string, std::string> &headers = data_->getHeaders();
  const std::map<std::string, Cookie> &cookies = data_->getCookies();

  ss << "HTTP/1.1 " << data_->getStatusCode() << " " << data_->getPhrase();
  ss << "\r\n";
  for (std::map<std::string, std::string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    ss << it->first << ": " << it->second << "\r\n";
  }
  for (std::map<std::string, Cookie>::const_iterator it = cookies.begin();
       it != cookies.end(); ++it) {
    ss << "Set-Cookie: " << it->second << "\r\n";
  }
  ss << "\r\n";
  ss << data_->getBody();
}

ResponseData &Response::getResponseData(void) { return *data_; }

void Response::setStatusLine(size_t status_code, const std::string &phrase) {
  data_->setStatusCode(status_code);
  data_->setPhrase(phrase);
}

void Response::insertHeader(const std::string &key, const std::string &value) {
  data_->insertHeader(key, value);
}

void Response::appendBody(const std::string &str) { data_->appendBody(str); }

void Response::resetResponseData(void) {
  delete this->data_;
  this->data_ = new ResponseData;
}

void Response::insertContentLength(void) {
  const size_t body_size = this->data_->getBody().size();
  std::stringstream ss;
  ss << body_size;
  data_->insertHeader("content-length", ss.str());
}

void Response::insertCommonHeaders(bool keep_alive) {
  this->insertHeader("Connection", (keep_alive ? "Keep-Alive" : "Close"));
  this->insertHeader("Server", "Webserv");
  this->insertContentLength();

  std::time_t raw_time;
  std::time(&raw_time);
  this->insertHeader("Date", HttpUtils::generateDateAsFormat(
                                 raw_time, "%a, %d %b %Y %H:%M:%S GMT"));
}

size_t Response::getStatusCode(void) const {
  return this->data_->getStatusCode();
}

bool Response::insertSetCookieHeader(
    const std::string &set_cookie_header_value) {
  return data_->insertCookie(set_cookie_header_value);
}

void Response::clearBody(void) { this->data_->clearBody(); }
