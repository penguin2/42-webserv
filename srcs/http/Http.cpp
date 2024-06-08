#include "Http.hpp"

#include <unistd.h>

#include <string>

#include "CgiRequest.hpp"
#include "CgiResponseHandler.hpp"
#include "ConnectionState.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/ServerConfig.hpp"

Http::Http(SocketAddress peer_address,
           const std::vector<const ServerConfig*>& server_configs)
    : peer_address_(peer_address),
      server_configs_(server_configs),
      keep_alive_flag_(true),
      cgi_request_(NULL),
      cgi_response_(NULL) {}

Http::~Http(void) { cleanupCgiResources(); }

connection::State Http::httpHandler(connection::State current_state) {
  connection::State next_state;

  switch (current_state) {
    case (connection::RECV):
      next_state = httpHandlerRecv();
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
  if (current_state == connection::RECV && next_state == connection::SEND) {
    prepareToSendResponse(this->response_);
  }
  if (current_state == connection::RECV && next_state == connection::CGI) {
    cleanupCgiResources();
    createCgiRequestAndResponse();
  }
  if ((current_state == connection::CGI ||
       current_state == connection::CGI_ERROR ||
       current_state == connection::CGI_TIMEOUT) &&
      next_state == connection::SEND) {
    prepareToSendResponse(*this->cgi_response_);
    cleanupCgiResources();
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

connection::State Http::httpHandlerRecv(void) {
  try {
    if (request_.parse(this->client_data_) == true) {
      Http::setServerConfig(request_, server_configs_);
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

void Http::prepareToSendResponse(Response& response) {
  this->keep_alive_flag_ =
      ((request_.haveConnectionCloseHeader() == false) &&
       HttpUtils::isMaintainConnection(response.getStatusCode()));
  if (response.getStatusCode() == 204) response.clearBody();
  response.insertCommonHeaders(this->keep_alive_flag_);
  response.getResponseRawData(raw_response_data_);
}

void Http::setServerConfig(
    Request& request, const std::vector<const ServerConfig*>& server_configs) {
  const std::string& host = request.getRequestData()->getUri().getHost();
  const ServerConfig* server_conf =
      ConfigAdapter::searchServerConfig(host, server_configs);

  if (server_conf == NULL)
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Internal Error");
  request.setServerConfig(*server_conf);
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
