#include "RequestData.hpp"

#include "HttpUtils.hpp"
#include "ServerException.hpp"
#include "Uri.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/ServerConfig.hpp"

RequestData::RequestData(void) : server_config(NULL) {}
RequestData::~RequestData(void) {}

void RequestData::setMethod(const std::string &method) {
  if (ConfigAdapter::getMaxMethodSize() < method.size()) {
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Method");
  }
  this->method_ = method;
}

void RequestData::setUri(const std::string &uri) {
  if (ConfigAdapter::getMaxUriSize() < uri.size()) {
    throw ServerException(ServerException::SERVER_ERROR_URI_TOO_LONG,
                          "Too long URI");
  }
  if (uri.empty()) {
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Request");
  }
  this->uri_.parse(uri);
}

void RequestData::setVersion(const std::string &version) { version_ = version; }

// line != "\0" が前提条件
void RequestData::insertHeader(std::string &line) {
  std::string key, value;
  const size_t pos_colon = line.find(':');

  // 先頭が空白文字の場合は行全体を無視する
  if (std::isspace(line[0])) return;

  // ':'が無い or keyが無い(": value"のような場合)
  if (pos_colon == std::string::npos || Utils::isStartsWith(line, ":"))
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad header");

  // Headerのkeyは大文字小文字を区別しないため、内部は全て小文字で処理
  key = Utils::toLower(line.substr(0, pos_colon));
  value = line.substr(pos_colon + 1);

  // valueの前後の空白は除去される
  Utils::strTrim(value, " ");
  // keyとコロンの間に空白文字がある場合
  if (std::isspace(key[key.size() - 1]))
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Header in space between key and colon");

  // keyに使用不可能文字が含まれる
  if (!Utils::isContainsOnly(key, HttpUtils::isHeaderKeyChar))
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Header contain unusable char");

  if (key == "host") {
    // hostヘッダが重複する場合
    if (headers_.count(key) != 0)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Multiple host header");
    // hostヘッダのvalueが無い場合
    if (value.size() == 0)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Bad host header");

    // Hostヘッダにuser_infoコンポーネントが含まれる
    if (Utils::isContain(value, "@"))
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Host header contain user_info");

    // URIがorigin-formの場合、Hostヘッダの値でURIの情報を上書き
    this->uri_.overwriteAuthorityIfNotSet(value);
  }
  // keyに空白文字が含まれる or hostヘッダ以外が重複する
  if (Utils::isContain(key, std::isspace) || headers_.count(key) != 0) return;
  headers_.insert(std::make_pair(key, value));
}

void RequestData::appendBody(const std::string &data) { body_.append(data); }

void RequestData::setServerConfig(const ServerConfig &server_conf) {
  this->server_config = &server_conf;
}

const std::string &RequestData::getMethod(void) const { return method_; }

const Uri &RequestData::getUri(void) const { return uri_; }

const std::string &RequestData::getVersion(void) const { return version_; }

const std::map<std::string, std::string> &RequestData::getHeaders(void) const {
  return headers_;
}

const std::string &RequestData::getBody(void) const { return body_; }

const ServerConfig *RequestData::getServerConfig(void) const {
  return server_config;
}

void RequestData::overwritePath(const std::string &new_path) {
  this->uri_.overwritePath(new_path);
}
