#include "Response.hpp"

#include "ResponseData.hpp"

Response::Response(void) { this->data_ = new ResponseData; }
Response::~Response(void) { delete this->data_; }

void Response::getResponseRawData(std::stringstream &ss) const {
  const std::map<std::string, std::string> headers = data_->getHeaders();

  ss << data_->getStatusCode() << " " << data_->getPhrase() << "\r\n";
  for (typename std::map<std::string, std::string>::const_iterator it =
           headers.begin();
       it != headers.end(); ++it) {
    ss << it->first << ": " << it->second << "\r\n";
  }
  ss << data_->getBody();
}

void Response::setStatusLine(int code, const std::string &phrase) {
  data_->setStatusCode(code);
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

void Response::insertContentLengthIfNotSet(void) {
  const std::map<std::string, std::string> headers = data_->getHeaders();
  if (headers.find("content-length") != headers.end()) return;
  const size_t body_size = data_->getBody().size();
  std::stringstream ss;
  ss << body_size;
  data_->insertHeader("content-length", ss.str());
}
