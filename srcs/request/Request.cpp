#include "Request.hpp"

#include <iostream>
#include <sstream>

#include "HttpException.hpp"
#include "RequestData.hpp"
#include "Uri.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "utils/Utils.hpp"

Request::Request(const std::vector<const ServerConfig*>& server_configs)
    : state_(METHOD),
      header_line_counter_(0),
      crlf_counter_before_method_(0),
      server_configs_(server_configs) {
  this->data_ = new RequestData;
}

Request::~Request(void) { delete this->data_; }

const RequestData* Request::getRequestData(void) const { return this->data_; }

void Request::overwritePath(const std::string& new_path) {
  this->data_->overwritePath(new_path);
}

bool Request::parse(std::string& buffer) {
  if (this->state_ == END) {
    this->state_ = METHOD;
    delete this->data_;
    this->data_ = new RequestData;
    header_line_counter_ = 0;
    crlf_counter_before_method_ = 0;
  }
  switch (this->state_) {
    case METHOD:
      parseMethod(buffer);
      break;
    case URI:
      parseUri(buffer);
      break;
    case VERSION:
      parseVersion(buffer);
      break;
    case HEADER:
      parseHeader(buffer);
      break;
    case BODY:
      parseBody(buffer);
      break;
    case CHUNKED_BODY:
      parseChunkedBody(buffer);
      break;
    case CHUNKED_SIZE:
      parseChunkedSize(buffer);
      break;
    case END:
      break;
  }
  return (this->state_ == END) ? true : false;
}

void Request::parseMethod(std::string& buffer) {
  // リクエストラインの前に多少のCRLFを置いて良い
  while (buffer.find("\r\n") == 0) {
    buffer.erase(0, 2);
    crlf_counter_before_method_++;
    if (ConfigAdapter::getMaxNumberOfCrlfBeforeMethod() <
        crlf_counter_before_method_)
      throw HttpException(HttpException::BAD_REQUEST,
                          "Too many CRLF before Method");
  }
  if (buffer.size() == 0 || (buffer.size() == 1 && buffer[0] == '\r')) return;
  const size_t pos_first_space = buffer.find(' ');
  // 対応するMethodの最大文字数よりも大きい
  if (pos_first_space == std::string::npos &&
      ConfigAdapter::getMaxMethodSize() < buffer.size())
    throw HttpException(HttpException::BAD_REQUEST, "Bad method");
  if (pos_first_space == std::string::npos) return;
  // StatusLineが空白で始まる
  if (pos_first_space == 0)
    throw HttpException(HttpException::BAD_REQUEST, "RequestLine start space");
  data_->setMethod(buffer.substr(0, pos_first_space));
  // SP分の+1
  buffer.erase(0, pos_first_space + 1);
  // メソッドが大文字でない
  if (!utils::isContainsOnly(data_->getMethod(), std::isupper))
    throw HttpException(HttpException::BAD_REQUEST, "Bad Method");
  this->state_ = URI;
  return parseUri(buffer);
}

void Request::parseUri(std::string& buffer) {
  const size_t pos_first_space = buffer.find(' ');
  // URIが大きすぎる
  if (pos_first_space == std::string::npos &&
      ConfigAdapter::getMaxUriSize() < buffer.size())
    throw HttpException(HttpException::URI_TOO_LONG, "Too long URI");
  if (pos_first_space == std::string::npos) return;
  // uriが不正な場合、RequestData内で例外をthrowする
  data_->setUri(buffer.substr(0, pos_first_space));
  // SP分の+1
  buffer.erase(0, pos_first_space + 1);
  this->state_ = VERSION;
  return parseVersion(buffer);
}

void Request::parseVersion(std::string& buffer) {
  static const std::string HTTP_VERSION = "HTTP/1.1";

  // CRLF分の+2
  if (buffer.size() < (HTTP_VERSION.size() + 2)) return;
  if (buffer.compare(0, 5, "HTTP/") != 0 || !std::isdigit(buffer[5]) ||
      buffer[6] != '.' || !std::isdigit(buffer[7]) ||
      buffer.compare(8, 2, "\r\n") != 0)
    throw HttpException(HttpException::BAD_REQUEST, "Bad request");
  data_->setVersion(buffer.substr(0, HTTP_VERSION.size()));
  buffer.erase(0, HTTP_VERSION.size() + 2);
  if (data_->getVersion() != HTTP_VERSION)
    throw HttpException(HttpException::HTTP_VERSION_NOT_SUPPORTED,
                        "Bad HTTP version");
  this->state_ = HEADER;
  return parseHeader(buffer);
}

void Request::parseHeader(std::string& buffer) {
  size_t pos_crlf;
  while ((pos_crlf = buffer.find("\r\n")) != std::string::npos) {
    // CRLFのみの行の場合、ヘッダの読み取りを終了
    if (pos_crlf == 0) {
      buffer.erase(0, 2);
      return determineParseBody(buffer);
    }

    std::string line = buffer.substr(0, pos_crlf);
    header_line_counter_++;
    // ヘッダの行が多すぎる(正しいフォーマットで記述されていないヘッダも含む)
    // or Headerの1行の文字数が多すぎる
    if (ConfigAdapter::getMaxNumberOfHeaders() < header_line_counter_ ||
        ConfigAdapter::getMaxHeaderSize() < line.size())
      throw HttpException(HttpException::HEADER_TOO_LARGE, "Header too large");

    // ヘッダエラーがあった場合、RequestData.insertHeaderメンバ関数内で例外がthrowされる
    data_->insertHeader(line);
    buffer.erase(0, pos_crlf + 2);
  }
  if (ConfigAdapter::getMaxHeaderSize() < buffer.size())
    throw HttpException(HttpException::HEADER_TOO_LARGE, "Header too large");
}

void Request::parseBody(std::string& buffer) {
  if (buffer.size() < this->body_size_) return;
  data_->appendBody(buffer.substr(0, body_size_));
  buffer.erase(0, body_size_);
  this->state_ = END;
}

void Request::parseChunkedBody(std::string& buffer) {
  // ボディは必ずCRLFで終了するため+2
  if (buffer.size() < (body_size_ + 2)) return;
  data_->appendBody(buffer.substr(0, body_size_));
  buffer.erase(0, body_size_);
  if (buffer.compare(0, 2, "\r\n"))
    throw HttpException(HttpException::BAD_REQUEST, "Bad chunk body");
  buffer.erase(0, 2);
  this->state_ = CHUNKED_SIZE;
  return parseChunkedSize(buffer);
}

void Request::parseChunkedSize(std::string& buffer) {
  static const size_t CHUNK_SIZE_LIMIT_DIGITS = 20;
  const size_t pos_crlf = buffer.find("\r\n");
  size_t chunk_size;
  const LocationConfig* location_config = ConfigAdapter::searchLocationConfig(
      data_->getUri().getPath(), getServerConfig()->getLocationConfigs());

  if (pos_crlf == std::string::npos && CHUNK_SIZE_LIMIT_DIGITS < buffer.size())
    throw HttpException(HttpException::BAD_REQUEST, "Bad chunk size");
  if (pos_crlf == std::string::npos) return;

  if (pos_crlf == 0)
    throw HttpException(HttpException::BAD_REQUEST, "Bad chunk size is CRLF");
  // 行頭からCRLFまでが符号なし16進数でない場合
  if (utils::strToSize_t(buffer.substr(0, pos_crlf), chunk_size, 16) == false)
    throw HttpException(HttpException::BAD_REQUEST, "Bad chunk size");

  // chunk_size or 合計のbodyのサイズが正常な数値であるが大き過ぎる場合
  if (ConfigAdapter::getClientMaxBodySize(*location_config) <
      (chunk_size + data_->getBody().size()))
    throw HttpException(HttpException::PAYLOAD_TOO_LARGE,
                        "Body size too large");

  this->body_size_ = chunk_size;
  buffer.erase(0, pos_crlf + 2);
  if (this->body_size_ != 0) {
    this->state_ = CHUNKED_BODY;
    return parseChunkedBody(buffer);
  } else {
    insertContentLength();
    this->state_ = END;
  }
}

void Request::determineParseBody(std::string& buffer) {
  const std::string& method = data_->getMethod();
  const std::map<std::string, std::string>& headers = data_->getHeaders();
  const Uri& uri = this->data_->getUri();
  const ServerConfig* server_config =
      ConfigAdapter::searchServerConfig(uri.getHost(), this->server_configs_);
  const std::map<std::string, std::string>::const_iterator host =
      headers.find("host");
  const std::map<std::string, std::string>::const_iterator transfer_encoding =
      headers.find("transfer-encoding");
  const std::map<std::string, std::string>::const_iterator content_length =
      headers.find("content-length");

  if (ConfigAdapter::isCorrespondingMethod(method) == false)
    throw HttpException(HttpException::BAD_REQUEST, "Bad Method");
  // hostヘッダフィールドが存在しない場合
  if (host == headers.end())
    throw HttpException(HttpException::BAD_REQUEST, "Host field not found");

  if (server_config == NULL)
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR, "Internal Error");
  setServerConfig(*server_config);

  const LocationConfig* location_config = ConfigAdapter::searchLocationConfig(
      uri.getPath(), server_config->getLocationConfigs());
  // ここでNULLcheckをするので以降Request.parse()内でsearchLocationConfig実行時,NULLが返ることは絶対にない
  if (location_config == NULL) {
    throw HttpException(HttpException::NOT_FOUND, "Not Found");
  }

  if (transfer_encoding != headers.end()) {
    // Transfer-Encodingにchunked以外の値がある場合
    if (transfer_encoding->second != "chunked")
      throw HttpException(HttpException::NOT_IMPLEMENTED,
                          "Unknown Transfer-Encoding");
    // chunkedとContent-Lengthが両方指定された場合
    if (content_length != headers.end())
      throw HttpException(HttpException::BAD_REQUEST,
                          "Exist chunked and Content-Length");
    this->state_ = CHUNKED_SIZE;
    return parseChunkedSize(buffer);
  } else if (content_length != headers.end()) {
    // 符号なし10進数でない
    if (utils::strToSize_t(content_length->second, body_size_, 10) == false)
      throw HttpException(HttpException::BAD_REQUEST, "Bad Content-Length");

    // body_size_が正常な数値であるが大き過ぎる場合
    if (ConfigAdapter::getClientMaxBodySize(*location_config) < body_size_)
      throw HttpException(HttpException::PAYLOAD_TOO_LARGE,
                          "Body size too large");

    this->state_ = BODY;
    return parseBody(buffer);
  } else {
    // Transfer-EncodingとContent-Length両方が存在しない場合はパース終了
    this->state_ = END;
  }
}

void Request::insertContentLength(void) {
  std::ostringstream oss;
  oss << "Content-Length: " << data_->getBody().size();
  std::string content_length_header = oss.str();
  data_->insertHeader(content_length_header);
}

bool Request::haveConnectionCloseHeader(void) const {
  return utils::isSameValueCaseInsensitive(this->data_->getHeaders(),
                                           "connection", "close");
}

void Request::setServerConfig(const ServerConfig& server_conf) {
  data_->setServerConfig(server_conf);
}

const ServerConfig* Request::getServerConfig(void) const {
  return data_->getServerConfig();
}
