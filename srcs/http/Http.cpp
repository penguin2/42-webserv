#include "Http.hpp"

#include <unistd.h>

#include <map>
#include <string>
#include <vector>

#include "ConnectionState.hpp"
#include "FileUtils.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"

Http::Http() : keep_alive_flag_(true) {}

Http::~Http() {}

connection::State Http::httpHandler(connection::State state) {
  switch (state) {
    case (connection::RECV):
      try {
        if (request_.parse(this->client_data_) == true) {
          return callRequestHandler();
        }
        return connection::RECV;
      } catch (ServerException& e) {
        return errorContentHandler(e.code(), e.what());
      }
    case (connection::SEND):
      if (!keep_alive_flag_) return connection::CLOSED;
      raw_response_data_.clear();
      raw_response_data_.str("");
      response_.resetResponseData();
      if (this->client_data_.size()) return Http::httpHandler(connection::RECV);
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
  this->keep_alive_flag_ = (request_.haveConnectionCloseHeader() == false &&
                            HttpUtils::isMaintainConnection(status_code));
  response_.insertCommonHeaders(this->keep_alive_flag_);
  response_.setStatusLine(status_code, phrase);
  response_.getResponseRawData(raw_response_data_);
  return connection::SEND;
}

connection::State Http::callRequestHandler(void) {
  connection::State next_state = RequestHandler::dispatch(request_, response_);
  this->keep_alive_flag_ =
      ((request_.haveConnectionCloseHeader() == false) &&
       HttpUtils::isMaintainConnection(this->response_.getStatusCode()));
  response_.insertCommonHeaders(this->keep_alive_flag_);
  response_.getResponseRawData(raw_response_data_);
  return next_state;
}
