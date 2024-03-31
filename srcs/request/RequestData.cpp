#include "RequestData.hpp"

#include "Uri.hpp"

RequestData::RequestData(void) {}
RequestData::~RequestData(void) {}

void RequestData::setMethod(const std::string &method) { method_ = method; }

void RequestData::setUri(const std::string &uri) { this->uri_.parse(uri); }

void RequestData::setVersion(const std::string &version) { version_ = version; }

void insertHeader(const std::string &line) {}

void RequestData::setBody(const std::string &body) { body_ = body; }

const std::string &RequestData::getMethod(void) const { return method_; }

const Uri &RequestData::getUri(void) const { return uri_; }

const std::string &RequestData::getVersion(void) const { return version_; }

const std::map<std::string, std::string> &RequestData::getHeaders(void) const {
  return headers_;
}

const std::string &RequestData::getBody(void) const { return body_; }
