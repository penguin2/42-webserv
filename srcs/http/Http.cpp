#include "Http.hpp"

#include <map>
#include <string>
#include <vector>

#include "ConfigAdapter.hpp"
#include "ConnectionState.hpp"
#include "HttpUtils.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"

Http::Http() : state_(Http::RECV), keep_alive_flag_(true) {}
Http::~Http() {}

connection::State Http::httpHandler() {
  switch (state_) {
    case (Http::RECV):
      try {
        if (request_.parse(this->client_data_) == true) {
          return dispatchRequestHandler();
        }
        return connection::RECV;
      } catch (ServerException& e) {
        return errorContentHandler(e.code(), e.what());
      }
    case (Http::SEND):
      if (keep_alive_flag_ == false) return connection::CLOSED;
      raw_response_data_.clear();
      raw_response_data_.str("");
      response_.resetResponseData();
      state_ = Http::RECV;
      if (this->client_data_.size()) return Http::httpHandler();
      return connection::RECV;
    default:
      break;
  }
  return connection::CLOSED;
}

void Http::appendClientData(const std::string& data) {
  client_data_.append(data);
}

std::string Http::getResponse() const { return raw_response_data_.str(); }

bool Http::haveConnectionCloseHeader(void) const {
  const std::map<std::string, std::string> headers =
      request_.getRequestData()->getHeaders();
  const std::map<std::string, std::string>::const_iterator connection_header =
      headers.find("connection");
  if (connection_header != headers.end()) {
    std::string connection_value(connection_header->second);
    Utils::toLowerString(connection_value);
    if (connection_value == "close") return true;
  }
  return false;
}

void Http::insertCommonHeaders(bool keep_alive) {
  this->keep_alive_flag_ = keep_alive;
  response_.insertHeader("Connection", (keep_alive ? "Keep-Alive" : "Close"));
  response_.insertHeader("Server", "Webserv");
  response_.insertHeader("Date", HttpUtils::generateDateValue());
}

connection::State Http::dispatchRequestHandler(void) {
  const Uri& uri = request_.getRequestData()->getUri();

  const std::string* redirect_uri = ConfigAdapter::searchRedirectUri(
      uri.getHost(), uri.getPort(), uri.getPath());
  if (redirect_uri != NULL) {
    return redirectHandler(redirect_uri);
  }
  // TODO if (拡張子がCGIプログラム) return cgiHandler();
  // TODO if (静的ファイルの要求) return staticContentHandler();
  {
    response_.appendBody(HttpUtils::readAllDataFromFile(uri.getPath()));
    response_.insertContentLengthIfNotSet();
    response_.insertHeader("Content-Type",
                           HttpUtils::convertPathToContentType(uri.getPath()));
    insertCommonHeaders(haveConnectionCloseHeader() == false);
    response_.setStatusLine(200, "OK");
    response_.getResponseRawData(raw_response_data_);
    this->state_ = Http::SEND;
    return connection::SEND;
  }
  // TODO if (どれにも当てはまらない == true)
  // throw ServerException(404, "Not Found");
}

connection::State Http::errorContentHandler(int status_code,
                                            const std::string& phrase) {
  const Uri& uri = request_.getRequestData()->getUri();
  const std::string* error_page =
      ConfigAdapter::searchErrorPage(uri.getHost(), uri.getPort(), status_code);

  response_.appendBody(
      HttpUtils::generateErrorPage(error_page, status_code, phrase));
  response_.insertHeader("Content-Type", "text/html");
  response_.insertContentLengthIfNotSet();
  // TODO Configクラスを見て何のメソッドが使用可能かを取得する処理
  if (status_code == 405) response_.insertHeader("Allow", "GET, POST, DELETE");
  insertCommonHeaders(haveConnectionCloseHeader() == false &&
                      HttpUtils::isMaintainConnection(status_code));
  response_.setStatusLine(status_code, phrase);
  response_.getResponseRawData(raw_response_data_);
  this->state_ = Http::SEND;
  return connection::SEND;
}

connection::State Http::redirectHandler(const std::string* redirect_uri) {
  const Uri& uri = request_.getRequestData()->getUri();
  const int redirect_status_code = ConfigAdapter::searchRedirectStatusCode(
      uri.getHost(), uri.getPort(), uri.getPath());
  const std::string* error_page = ConfigAdapter::searchErrorPage(
      uri.getHost(), uri.getPort(), redirect_status_code);

  if (HttpUtils::isRedirectStatusCode(redirect_status_code) == false)
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Return directive is invalid");

  response_.appendBody(HttpUtils::generateErrorPage(
      error_page, redirect_status_code, "Redirect"));
  insertCommonHeaders(haveConnectionCloseHeader() == false &&
                      HttpUtils::isMaintainConnection(redirect_status_code));
  response_.insertHeader("Location", *redirect_uri);
  response_.setStatusLine(redirect_status_code, "Redirect");
  response_.getResponseRawData(raw_response_data_);
  this->state_ = Http::SEND;
  return connection::SEND;
}
