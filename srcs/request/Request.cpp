#include "Request.hpp"

#include <iostream>
#include <sstream>

#include "RequestData.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"

Request::Request(void)
    : state_(METHOD), header_line_counter_(0), crlf_counter_before_method_(0) {
  this->data_ = new RequestData;
}

Request::~Request(void) { delete this->data_; }

const RequestData* Request::getRequestData(void) const { return this->data_; }

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
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Too many CRLF before Method");
  }
  if (buffer.size() == 0 || (buffer.size() == 1 && buffer[0] == '\r')) return;
  const size_t pos_first_space = buffer.find(' ');
  // 対応するMethodの最大文字数よりも大きい
  if (pos_first_space == std::string::npos &&
      ConfigAdapter::getMaxMethodSize() < buffer.size())
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad method");
  if (pos_first_space == std::string::npos) return;
  // StatusLineが空白で始まる
  if (pos_first_space == 0)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "RequestLine start space");
  data_->setMethod(buffer.substr(0, pos_first_space));
  // SP分の+1
  buffer.erase(0, pos_first_space + 1);
  // メソッドが大文字でない
  if (!Utils::isContainsOnly(data_->getMethod(), std::isupper))
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Method");
  this->state_ = URI;
  return parseUri(buffer);
}

void Request::parseUri(std::string& buffer) {
  const size_t pos_first_space = buffer.find(' ');
  // URIが大きすぎる
  if (pos_first_space == std::string::npos &&
      ConfigAdapter::getMaxUriSize() < buffer.size())
    throw ServerException(ServerException::SERVER_ERROR_URI_TOO_LONG,
                          "Too long URI");
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
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad request");
  data_->setVersion(buffer.substr(0, HTTP_VERSION.size()));
  buffer.erase(0, HTTP_VERSION.size() + 2);
  if (data_->getVersion() != HTTP_VERSION)
    throw ServerException(
        ServerException::SERVER_ERROR_HTTP_VERSION_NOT_SUPPORTED,
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
      throw ServerException(ServerException::SERVER_ERROR_HEADER_TOO_LARGE,
                            "Header too large");

    // ヘッダエラーがあった場合、RequestData.insertHeaderメンバ関数内で例外がthrowされる
    data_->insertHeader(line);
    buffer.erase(0, pos_crlf + 2);
  }
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
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad chunk body");
  buffer.erase(0, 2);
  this->state_ = CHUNKED_SIZE;
  return parseChunkedSize(buffer);
}

void Request::parseChunkedSize(std::string& buffer) {
  const size_t pos_crlf = buffer.find("\r\n");
  if (pos_crlf == std::string::npos) return;
  if (pos_crlf == 0)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad chunk size is CRLF");
  // 行頭からCRLFまでが符号なし16進数でない場合
  if (Utils::strToSize_t(buffer.substr(0, pos_crlf), body_size_, 16) == false)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad chunk size");

  // body_size_ or 合計のbodyのサイズが正常な数値であるが大き過ぎる場合
  const Uri& uri = data_->getUri();
  if (ConfigAdapter::getMaxBodySize(uri.getHost(), uri.getPort(),
                                    uri.getPath()) <
      (body_size_ + data_->getBody().size()))
    throw ServerException(ServerException::SERVER_ERROR_PAYLOAD_TOO_LARGE,
                          "Body size too large");

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
  const std::map<std::string, std::string>::const_iterator host =
      headers.find("host");
  const std::map<std::string, std::string>::const_iterator transfer_encoding =
      headers.find("transfer-encoding");
  const std::map<std::string, std::string>::const_iterator content_length =
      headers.find("content-length");

  if (ConfigAdapter::isCorrespondingMethod(method) == false)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Method");
  // hostヘッダフィールドが存在しない場合
  if (host == headers.end())
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Host field not found");

  if (transfer_encoding != headers.end()) {
    // Transfer-Encodingにchunked以外の値がある場合
    if (transfer_encoding->second != "chunked")
      throw ServerException(ServerException::SERVER_ERROR_NOT_IMPLEMENTED,
                            "Unknown Transfer-Encoding");
    // chunkedとContent-Lengthが両方指定された場合
    if (content_length != headers.end())
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Exist chunked and Content-Length");
    this->state_ = CHUNKED_SIZE;
    return parseChunkedSize(buffer);
  } else if (content_length != headers.end()) {
    // 符号なし10進数でない
    if (Utils::strToSize_t(content_length->second, body_size_, 10) == false)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Bad Content-Length");

    // body_size_が正常な数値であるが大き過ぎる場合
    const Uri& uri = data_->getUri();
    if (ConfigAdapter::getMaxBodySize(uri.getHost(), uri.getPort(),
                                      uri.getPath()) < body_size_)
      throw ServerException(ServerException::SERVER_ERROR_PAYLOAD_TOO_LARGE,
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
  return Utils::isSameValueCaseInsensitive(this->data_->getHeaders(),
                                           "connection", "close");
}
