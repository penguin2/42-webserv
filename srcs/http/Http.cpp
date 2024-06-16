#include "Http.hpp"

#include <unistd.h>

#include <string>

#include "CgiRequest.hpp"
#include "CgiResponseHandler.hpp"
#include "ConnectionState.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/ServerConfig.hpp"
#include "utils/HttpUtils.hpp"
#include "utils/Utils.hpp"

Http::Http(SocketAddress peer_address,
           const std::vector<const ServerConfig*>& server_configs)
    : peer_address_(peer_address),
      server_configs_(server_configs),
      keep_alive_flag_(true),
      request_(server_configs),
      cgi_request_(NULL),
      cgi_response_(NULL) {}

Http::~Http(void) { cleanupCgiResources(); }

connection::State Http::httpHandler(connection::State current_state) {
  connection::State next_state;

  switch (current_state) {
    case (connection::RECV):
      next_state = httpHandlerRecv();
      break;
    case (connection::RECV_TIMEOUT):
      next_state = httpHandlerRecvTimeout();
      break;
    case (connection::SEND):
      next_state = httpHandlerSend();
      break;
    case (connection::CGI):
      next_state = httpHandlerCgi();
      break;
    case (connection::CGI_ERROR):
      next_state = httpHandlerCgiError();
      break;
    case (connection::CGI_TIMEOUT):
      next_state = httpHandlerCgiTimeout();
      break;
    default:
      next_state = connection::CLOSED;
      break;
  }
  if ((current_state == connection::RECV ||
       current_state == connection::RECV_TIMEOUT ||
       current_state == connection::SEND) &&
      next_state == connection::SEND) {
    prepareToSendResponse(this->response_);
  }
  if ((current_state == connection::RECV ||
       current_state == connection::SEND) &&
      next_state == connection::CGI) {
    cleanupCgiResources();
    createCgiRequestAndResponse();
  }
  if ((current_state == connection::CGI ||
       current_state == connection::CGI_ERROR ||
       current_state == connection::CGI_TIMEOUT) &&
      next_state == connection::SEND) {
    prepareToSendResponse(*this->cgi_response_);
  }
  return next_state;
}

void Http::appendClientData(const std::string& data) {
  client_data_.append(data);
}

std::string Http::getResponse() const { return raw_response_data_.str(); }

CgiRequest* Http::getCgiRequest() const { return cgi_request_; }

void Http::setCgiResponseMessage(const std::string& message) {
  cgi_response_message_ = message;
}

std::string Http::makeResponseLog() const {
  const Response& current_response =
      cgi_response_ != NULL ? *cgi_response_ : response_;
  const std::string& method = request_.getRequestData()->getMethod();
  const std::string& uri =
      request_.getRequestData()->getUri().getHost().empty()
          ? ""
          : request_.getRequestData()->getUri().buildAbsoluteUri();
  return utils::uintToString(current_response.getStatusCode()) + " \"" +
         method + "\" \"" + uri + "\"";
}

connection::State Http::httpHandlerRecv(void) {
  try {
    if (request_.parse(this->client_data_) == true) {
      return RequestHandler::dispatch(
          request_, response_, request_.getRequestData()->getUri().getPath());
    }
    return connection::RECV;
  } catch (ServerException& e) {
    return RequestHandler::errorRequestHandler(request_, response_, e.code(),
                                               e.what());
  }
}

connection::State Http::httpHandlerSend(void) {
  if (!keep_alive_flag_) return connection::CLOSED;
  raw_response_data_.clear();
  raw_response_data_.str("");
  response_.resetResponseData();
  if (this->client_data_.empty()) return connection::RECV;
  return Http::httpHandlerRecv();
}

connection::State Http::httpHandlerCgi(void) {
  try {
    if (cgi_response_->parse(cgi_response_message_) == true) {
      CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
          request_, cgi_response_->Response::getResponseData());
    } else {
      throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                            "Cgi Parse Error");
    }
  } catch (ServerException& e) {
    this->cgi_response_->resetResponseData();
    return RequestHandler::errorRequestHandler(request_, *cgi_response_,
                                               e.code(), e.what());
  }
  return connection::SEND;
}

connection::State Http::httpHandlerCgiError(void) {
  this->cgi_response_->resetResponseData();
  return RequestHandler::errorRequestHandler(request_, *cgi_response_, 500,
                                             "Internal Server Error");
}

connection::State Http::httpHandlerCgiTimeout(void) {
  this->cgi_response_->resetResponseData();
  return RequestHandler::errorRequestHandler(request_, *cgi_response_, 504,
                                             "Gateway Timeout");
}

connection::State Http::httpHandlerRecvTimeout(void) {
  // errorRequestHandler内でrequest_が不完全な状態にあることによる不都合は無い
  return RequestHandler::errorRequestHandler(request_, response_, 408,
                                             "Request Timeout");
}

void Http::prepareToSendResponse(Response& response) {
  this->keep_alive_flag_ =
      ((request_.haveConnectionCloseHeader() == false) &&
       http_utils::isKeepConnection(response.getStatusCode()));
  if (response.getStatusCode() == 204) response.clearBody();
  response.insertCommonHeaders(this->keep_alive_flag_);
  response.getResponseRawData(raw_response_data_);
}

void Http::createCgiRequestAndResponse(void) {
  this->cgi_request_ = CgiRequest::createCgiRequest(request_, peer_address_);
  this->cgi_response_ = new CgiResponse;
}

void Http::cleanupCgiResources(void) {
  if (!this->cgi_response_message_.empty()) {
    this->cgi_response_message_.clear();
  }
  if (this->cgi_request_ != NULL) {
    delete this->cgi_request_;
    this->cgi_request_ = NULL;
  }
  if (this->cgi_response_ != NULL) {
    delete this->cgi_response_;
    this->cgi_response_ = NULL;
  }
}
