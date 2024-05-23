#include "ResponseData.hpp"

#include "Utils.hpp"

ResponseData::ResponseData(void) : status_code_(0) {}
ResponseData::~ResponseData(void) {}

int ResponseData::getStatusCode(void) const { return this->status_code_; }

const std::string& ResponseData::getPhrase(void) const { return this->phrase_; }

const std::map<std::string, std::string>& ResponseData::getHeaders(void) const {
  return this->headers_;
}

const std::string& ResponseData::getBody(void) const { return this->body_; }

void ResponseData::setStatusCode(int code) { status_code_ = code; }

void ResponseData::setPhrase(const std::string& phrase) { phrase_ = phrase; }

void ResponseData::appendBody(const std::string& str) { body_.append(str); }

void ResponseData::insertHeader(const std::string& key,
                                const std::string& value) {
  // Headerのkeyは大文字小文字を区別しないため内部で小文字で管理
  std::string lower_case_key = Utils::toLower(key);
  // 既に存在していれば上書きする
  headers_[lower_case_key] = value;
}

void ResponseData::eraseHeader(const std::string& key) { headers_.erase(key); }
