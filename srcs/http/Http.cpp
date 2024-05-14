#include "Http.hpp"

#include <unistd.h>

#include <string>

#include "ConnectionState.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerConfig.hpp"
#include "ServerException.hpp"
#include "config/ConfigAdapter.hpp"

Http::Http(SocketAddress peer_address,
           const std::vector<const ServerConfig*>& server_configs)
    : peer_address_(peer_address),
      server_configs_(server_configs),
      keep_alive_flag_(true) {}

Http::~Http() {}

connection::State Http::httpHandler(connection::State current_state) {
  connection::State next_state;

  switch (current_state) {
    case (connection::RECV):
      next_state = httpHandlerRecv();
      break;
    case (connection::SEND):
      next_state = httpHandlerSend();
      break;
      // TODO case (connection::CGI):
      // next_state = httpHandlerCGI();
      // break;
    default:
      next_state = connection::CLOSED;
      break;
  }
  if (next_state == connection::SEND) {
    prepareToSendResponse();
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

void Http::prepareToSendResponse(void) {
  this->keep_alive_flag_ =
      ((request_.haveConnectionCloseHeader() == false) &&
       HttpUtils::isMaintainConnection(this->response_.getStatusCode()));
  response_.insertCommonHeaders(this->keep_alive_flag_);
  response_.getResponseRawData(raw_response_data_);
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
