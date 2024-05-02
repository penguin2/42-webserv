#include "Http.hpp"

#include <unistd.h>

#include <string>

#include "ConnectionState.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"

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
        return callRequestHandler(e.code(), e.what());
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

connection::State Http::callRequestHandler(int status_code,
                                           const std::string& phrase) {
  connection::State next_state = RequestHandler::errorRequestHandler(
      request_, response_, status_code, phrase);
  this->keep_alive_flag_ =
      ((request_.haveConnectionCloseHeader() == false) &&
       HttpUtils::isMaintainConnection(this->response_.getStatusCode()));
  response_.insertCommonHeaders(this->keep_alive_flag_);
  response_.getResponseRawData(raw_response_data_);
  return next_state;
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
