#include "HttpMock.hpp"

HttpMock::HttpMock() : state_(HttpMock::RECV) {}

HttpMock::~HttpMock() {}

connection::State HttpMock::httpHandler() {
  switch (state_) {
    case HttpMock::RECV:
      if (checkResponse()) {
        makeResponse();
        state_ = HttpMock::SEND;
        return connection::SEND;
      }
      return connection::RECV;
    case HttpMock::SEND:
      raw_response_data_.clear();
      raw_response_data_.str("");
      state_ = HttpMock::RECV;
      return connection::RECV;
    default:
      break;
  }
  return connection::CLOSED;
}

void HttpMock::appendClientData(const std::string& data) {
  client_data_.append(data);
}

std::string HttpMock::getResponse() const { return raw_response_data_.str(); }

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
