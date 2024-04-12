#include "Http.hpp"

#include <map>
#include <string>
#include <vector>

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
        if (request_.parse(this->client_data_) == true) return dispatch();
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

connection::State Http::errorContentHandler(int status_code,
                                            const std::string& phrase) {
  response_.setStatusLine(status_code, phrase);
  // TODO Serverの値をConfigクラスから取得できると良さそう
  response_.insertHeader("Server", "Webserv");
  response_.insertHeader("Date", HttpUtils::generateDateValue());
  response_.insertHeader("Content-Type", "text/html");
  // TODO Configクラスを見てdefaultエラーページがある場合、そこから読み取る処理
  response_.appendBody(HttpUtils::generateErrorPage(status_code, phrase));
  response_.insertContentLengthIfNotSet();
  // TODO Configクラスを見て何のメソッドが使用可能かを取得する処理
  if (status_code == 405) response_.insertHeader("Allow", "GET, POST, DELETE");

  // TODO ConnectionをCloseするStatusCodeを調べる
  insertConnectionHeader(
      isConnectionKeepAlive() &&
      (status_code != 400 && status_code != 414 && status_code != 501));
  response_.getResponseRawData(raw_response_data_);
  this->state_ = Http::SEND;
  return connection::SEND;
}

bool Http::isConnectionKeepAlive(void) const {
  const std::map<std::string, std::string> headers =
      request_.getRequestData()->getHeaders();
  const std::map<std::string, std::string>::const_iterator connection_header =
      headers.find("connection");
  if (connection_header != headers.end()) {
    std::string connection_value(connection_header->second);
    Utils::toLowerString(connection_value);
    if (connection_value == "close") return false;
  }
  return true;
}

void Http::insertConnectionHeader(bool keep_alive) {
  if (keep_alive) {
    this->keep_alive_flag_ = true;
    response_.insertHeader("Connection", "Keep-Alive");
  } else {
    this->keep_alive_flag_ = false;
    response_.insertHeader("Connection", "Close");
  }
}

connection::State Http::dispatch(void) {
  const std::string& path = request_.getRequestData()->getUri().getPath();

  // TODO if (拡張子がCGIプログラム) return cgiHandler();
  // TODO if (リダイレクトすべきpath) return redirectHandler();
  // TODO if (静的ファイルの要求) return staticContentHandler();
  {
    response_.appendBody(HttpUtils::generatePage(path));
    response_.setStatusLine(200, "OK");
    response_.insertHeader("Server", "Webserv");
    response_.insertHeader("Date", HttpUtils::generateDateValue());
    response_.insertHeader("Content-Type",
                           HttpUtils::generateContentType(path));
    insertConnectionHeader(isConnectionKeepAlive());
    response_.getResponseRawData(raw_response_data_);
    this->state_ = Http::SEND;
    return connection::SEND;
  }
  // TODO if (どれにも当てはまらない == true)
  // throw ServerException(404, "Not Found");
}
