#include "CgiResponse.hpp"

#include "HttpException.hpp"
#include "Uri.hpp"
#include "utils/HttpUtils.hpp"
#include "utils/Utils.hpp"

CgiResponse::CgiResponse(void) : Response(), state_(HEADER) {}
CgiResponse::~CgiResponse(void) {}

bool CgiResponse::parse(std::string& buffer) {
  switch (this->state_) {
    case (HEADER):
      parseHeader(buffer);
      break;
    case (BODY):
      parseBody(buffer);
      break;
    default:
      break;
  }
  return (this->state_ == BODY);
}

void CgiResponse::parseHeader(std::string& buffer) {
  size_t pos_crlf;
  while ((pos_crlf = buffer.find('\n')) != std::string::npos) {
    // LFのみの行の場合、ヘッダの読み取りを終了
    if (pos_crlf == 0) {
      buffer.erase(0, 1);
      this->state_ = BODY;
      return parseBody(buffer);
    }
    std::string line = buffer.substr(0, pos_crlf);
    insertHeaderLine(line);
    buffer.erase(0, pos_crlf + 1);
  }
}

void CgiResponse::parseBody(std::string& buffer) {
  this->appendBody(buffer);
  buffer.clear();
}

void CgiResponse::insertHeaderLine(const std::string& line) {
  std::string key, value;
  const size_t pos_colon = line.find(':');

  // 先頭が空白文字の場合は行全体を無視する
  if (std::isspace(line[0])) return;

  // ':'が無い or keyが無い(": value"のような場合)
  if (pos_colon == std::string::npos || utils::isStartsWith(line, ":"))
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR, "Bad header");

  // Headerのkeyは大文字小文字を区別しないため、内部は全て小文字で処理
  key = utils::toLower(line.substr(0, pos_colon));
  value = line.substr(pos_colon + 1);

  // valueの前後の空白は除去される
  utils::strTrim(value, " ");
  // keyとコロンの間に空白文字がある場合
  if (std::isspace(key[key.size() - 1]))
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "CGI Header in space between key and colon");

  // keyに使用不可能文字が含まれる or value 無し
  if (!utils::isContainsOnly(key, http_utils::isHeaderKeyChar) || value.empty())
    return;

  // if (Multiple header) CGI-field -> ERROR, Other-field -> ignore
  if (utils::hasContentInMap(data_->getHeaders(), key)) {
    if (key == "location" || key == "status" || key == "content-type") {
      throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                          "Multiple CGI-field");
    } else {
      return;
    }
  }

  if (key == "location")
    insertLocationHeader(value);
  else if (key == "status")
    insertStatusHeader(line);
  else if (key == "content-type")
    insertContentTypeHeader(value);
  else if (key == "set-cookie")
    insertSetCookieHeader(value);
  else
    insertHeader(key, value);
}

void CgiResponse::insertStatusHeader(const std::string& line) {
  std::string value = line.substr(line.find(':') + 1);
  utils::strLTrim(value, " ");
  size_t pos_first_space = value.find(' ');
  if (pos_first_space == std::string::npos)
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Status Header Value is invalid");

  const std::string code_str = value.substr(0, pos_first_space);
  size_t status_code;
  if (utils::strToSize_t(code_str, status_code, 10) == false ||
      !http_utils::isStatusCode(status_code))
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Status Header is invalid");

  std::string phrase = value.substr(pos_first_space + 1);
  if (!utils::isContainsOnly(phrase, std::isprint))
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Phrase contain non-printable character");
  utils::strTrim(phrase, " ");

  insertHeader("status", utils::uintToString(status_code) + " " + phrase);
}

void CgiResponse::insertLocationHeader(const std::string& value) {
  Uri uri;

  try {
    uri.parse(value);
    insertHeader("location", value);
  } catch (HttpException& e) {
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Location Header Value is invalid");
  }
}

void CgiResponse::insertContentTypeHeader(const std::string& value) {
  size_t pos_slash = value.find('/');

  if (pos_slash == std::string::npos)
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Invalid Content-Type value");

  std::string type = value.substr(0, pos_slash);
  std::string subtype = value.substr(pos_slash + 1);

  if (type.empty() || subtype.empty())
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Invalid Content-Type value");
  insertHeader("content-type", value);
}
