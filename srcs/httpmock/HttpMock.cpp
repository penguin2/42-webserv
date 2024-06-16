#include "HttpMock.hpp"

#include "ConnectionState.hpp"
#include "utils/Utils.hpp"

const std::string HttpMock::kCgiCheckString = "CGI\r\n";

HttpMock::HttpMock(SocketAddress peer_address,
                   const std::vector<const ServerConfig*>& server_configs)
    : cgi_request_(NULL) {
  (void)peer_address;
  (void)server_configs;
}

HttpMock::~HttpMock() { clearCgiHandling(); }

connection::State HttpMock::httpHandler(connection::State state) {
  switch (state) {
    case connection::RECV:
      if (isCgi()) {
        std::string client_request = utils::popFrontSubstr(
            client_data_, HttpMock::kCgiCheckString.size());
        cgi_request_ = CgiRequestMock::createCgiRequest(client_request);
        if (cgi_request_ == NULL) {
          raw_response_data_ << "ECHO/1.0 502 Bad Gateway\n";
          return connection::SEND;
        }
        return connection::CGI;
      } else if (checkResponse()) {
        makeResponse();
        return connection::SEND;
      }
      return connection::RECV;
    case connection::SEND:
      raw_response_data_.clear();
      raw_response_data_.str("");
      return connection::RECV;
    case connection::CGI:
      makeCgiResponse();
      clearCgiHandling();
      return connection::SEND;
    default:
      break;
  }
  return connection::CLOSED;
}

void HttpMock::appendClientData(const std::string& data) {
  client_data_.append(data);
}

std::string HttpMock::getResponse() const { return raw_response_data_.str(); }

CgiRequestMock* HttpMock::getCgiRequest() const { return cgi_request_; }

void HttpMock::setCgiResponseMessage(const std::string& message) {
  cgi_response_message_ = message;
}

bool HttpMock::checkResponse() const {
  return client_data_.find('\n') != std::string::npos;
}

void HttpMock::makeResponse() {
  const std::string::size_type new_line_index = client_data_.find('\n');
  if (new_line_index == std::string::npos) {
    raw_response_data_ << "Error 404\n";
    return;
  }
  raw_response_data_ << std::string("ECHO/1.0 200 OK\n")
                     << client_data_.substr(0, new_line_index + 1);
  client_data_.erase(0, new_line_index + 1);
  return;
}

bool HttpMock::isCgi() const {
  return utils::isStartsWith(client_data_, HttpMock::kCgiCheckString);
}

void HttpMock::makeCgiResponse() {
  raw_response_data_ << "----- (from cgi-script) -----\n";
  raw_response_data_ << cgi_response_message_;
  raw_response_data_ << "-----------------------------\n";
}

void HttpMock::clearCgiHandling() {
  if (cgi_request_ != NULL) {
    delete cgi_request_;
    cgi_request_ = NULL;
  }
  cgi_response_message_.clear();
}
