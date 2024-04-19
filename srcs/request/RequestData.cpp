#include "RequestData.hpp"

#include "ConfigAdapter.hpp"
#include "ServerException.hpp"
#include "Uri.hpp"
#include "Utils.hpp"

RequestData::RequestData(void) {}
RequestData::~RequestData(void) {}

void RequestData::setMethod(const std::string &method) { method_ = method; }

void RequestData::setUri(const std::string &uri) { this->uri_.parse(uri); }

void RequestData::setVersion(const std::string &version) { version_ = version; }

// line != "\0" が前提条件
void RequestData::insertHeader(std::string &line) {
  std::string key, value;
  const size_t pos_colon = line.find(':');

  // Headerの1行の文字数が多すぎる
  if (ConfigAdapter::getMaxHeaderSize() < line.size())
    throw ServerException(ServerException::SERVER_ERROR_HEADER_TOO_LARGE,
                          "Header too large");

  // 先頭が空白文字の場合は行全体を無視する
  if (std::isspace(line[0])) return;

  if (pos_colon == std::string::npos) {
    key = line;
  } else {
    key = line.substr(0, pos_colon);
    value = line.substr(pos_colon + 1);
    // valueの前後の空白は除去される
    Utils::strTrim(value, " ");
    // keyが無い(": value"のような場合)
    if (key.size() == 0) return;
    // keyとコロンの間に空白文字がある場合
    if (std::isspace(key[key.size() - 1]))
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Header in space between key and colon");
  }

  // Headerのkeyは大文字小文字を区別しないため、内部は全て小文字で処理
  Utils::toLowerString(key);

  if (key == "host") {
    // hostヘッダが重複する場合
    if (headers_.count(key) != 0)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Multiple host header");
    // hostヘッダのvalueが無い場合
    if (value.size() == 0)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Bad host header");
    // URIがorigin-formの場合、Hostヘッダの値でURIの情報を上書き
    this->uri_.overwriteAuthorityIfNotSet(value);
  }
  // keyに空白文字が含まれる or hostヘッダ以外が重複する
  if (Utils::isContain(key, std::isspace) || headers_.count(key) != 0) return;
  headers_.insert(std::make_pair(key, value));
}

void RequestData::appendBody(const std::string &data) { body_.append(data); }

const std::string &RequestData::getMethod(void) const { return method_; }

const Uri &RequestData::getUri(void) const { return uri_; }

const std::string &RequestData::getVersion(void) const { return version_; }

const std::map<std::string, std::string> &RequestData::getHeaders(void) const {
  return headers_;
}

const std::string &RequestData::getBody(void) const { return body_; }
