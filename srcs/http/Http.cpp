#include "Http.hpp"

#include <unistd.h>

#include <map>
#include <string>
#include <vector>

#include "ConnectionState.hpp"
#include "FileUtils.hpp"
#include "HttpUtils.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"

Http::Http() : state_(Http::RECV), keep_alive_flag_(true) {
  method_handler_map_["GET"] = &Http::getMethodHandler;
  method_handler_map_["POST"] = &Http::postMethodHandler;
  method_handler_map_["DELETE"] = &Http::deleteMethodHandler;
}
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
      if (!keep_alive_flag_) return connection::CLOSED;
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

CgiRequest* Http::getCgiRequest() const { return cgi_request_; }

void Http::setCgiResponseMessage(const std::string& message) {
  cgi_response_message_ = message;
}

bool Http::haveConnectionCloseHeader(void) const {
  const std::map<std::string, std::string>& headers =
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

  std::time_t raw_time;
  std::time(&raw_time);
  response_.insertHeader("Date", HttpUtils::generateDateAsFormat(
                                     raw_time, "%a, %d %b %Y %H:%M:%S GMT"));
}

connection::State Http::dispatchRequestHandler(void) {
  const Uri& uri = request_.getRequestData()->getUri();

  if (!ConfigAdapter::isAllowMethods(uri.getHost(), uri.getPort(),
                                     uri.getPath(),
                                     request_.getRequestData()->getMethod()))
    throw ServerException(ServerException::SERVER_ERROR_METHOD_NOT_ALLOWED,
                          "Method not allowed");

  const std::string* redirect_uri = ConfigAdapter::searchRedirectUri(
      uri.getHost(), uri.getPort(), uri.getPath());
  if (redirect_uri != NULL) {
    return redirectHandler(*redirect_uri);
  }
  // TODO CGI Handle
  // else if (ConfigAdapter::isCgiPath(uri.getHost(), uri.getPort(),
  //   		  uri.getPath())) {
  //     return cgiHandler();
  // }
  else {
    return staticContentHandler();
  }
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
  if (status_code == 405) {
    const Uri& uri = request_.getRequestData()->getUri();
    const std::vector<std::string> allow_methods =
        ConfigAdapter::getAllowMethods(uri.getHost(), uri.getPort(),
                                       uri.getPath());
    response_.insertHeader("Allow", Utils::joinStrings(allow_methods, " ,"));
  }
  insertCommonHeaders(haveConnectionCloseHeader() == false &&
                      HttpUtils::isMaintainConnection(status_code));
  response_.setStatusLine(status_code, phrase);
  response_.getResponseRawData(raw_response_data_);
  this->state_ = Http::SEND;
  return connection::SEND;
}

connection::State Http::redirectHandler(const std::string& redirect_uri) {
  const Uri& uri = request_.getRequestData()->getUri();
  const int redirect_status_code = ConfigAdapter::searchRedirectStatusCode(
      uri.getHost(), uri.getPort(), uri.getPath());

  if (!HttpUtils::isRedirectStatusCode(redirect_status_code))
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Return directive is invalid");
  const std::string* error_page = ConfigAdapter::searchErrorPage(
      uri.getHost(), uri.getPort(), redirect_status_code);
  response_.appendBody(HttpUtils::generateErrorPage(
      error_page, redirect_status_code, "Redirect"));
  insertCommonHeaders(haveConnectionCloseHeader() == false &&
                      HttpUtils::isMaintainConnection(redirect_status_code));
  response_.insertContentLengthIfNotSet();
  response_.insertHeader("Location", redirect_uri);
  response_.setStatusLine(redirect_status_code, "Redirect");
  response_.getResponseRawData(raw_response_data_);
  this->state_ = Http::SEND;
  return connection::SEND;
}

connection::State Http::staticContentHandler(void) {
  const std::string& method = request_.getRequestData()->getMethod();

  if (method_handler_map_.find(method) == method_handler_map_.end()) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Unknown method");
  }
  return (this->*method_handler_map_[method])();
}

// (仮)
connection::State Http::getMethodHandler(void) {
  const Uri& uri = request_.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  if (!FileUtils::isExistFile(uri.getPath()))
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND,
                          "File not Found");
  if (!FileUtils::hasFilePermission(uri.getPath(), R_OK))
    throw ServerException(ServerException::SERVER_ERROR_FORBIDDEN,
                          "Has not permission");
  std::stringstream ss;
  if (FileUtils::readAllDataFromFile(uri.getPath(), ss) == false)
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Internal Server Error");
  response_.appendBody(ss.str());
  response_.insertContentLengthIfNotSet();
  response_.insertHeader("Content-Type",
                         HttpUtils::convertPathToContentType(uri.getPath()));
  insertCommonHeaders(haveConnectionCloseHeader() == false);
  response_.setStatusLine(200, "OK");
  response_.getResponseRawData(raw_response_data_);
  this->state_ = Http::SEND;
  return connection::SEND;
  (void)paths;
}

// (仮)
connection::State Http::postMethodHandler(void) {
  const Uri& uri = request_.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  this->state_ = Http::SEND;
  return connection::SEND;
  (void)paths;
}

// (仮)
connection::State Http::deleteMethodHandler(void) {
  const Uri& uri = request_.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  this->state_ = Http::SEND;
  return connection::SEND;
  (void)paths;
}
