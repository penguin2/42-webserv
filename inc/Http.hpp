#ifndef WEBSERV_HTTP_H_
#define WEBSERV_HTTP_H_

#include <sstream>
#include <string>

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "SocketAddress.hpp"
#include "config/ServerConfig.hpp"

// TODO: CgiRequestMock -> CgiRequest
// #include "CgiRequest.hpp"
#include "CgiRequestMock.hpp"

class Http {
 public:
  Http(SocketAddress peer_address,
       const std::vector<const ServerConfig*>& server_configs);
  ~Http(void);

  connection::State httpHandler(connection::State state);
  void appendClientData(const std::string& data);
  std::string getResponse(void) const;

  CgiRequest* getCgiRequest() const;
  void setCgiResponseMessage(const std::string& message);

 private:
  SocketAddress peer_address_;
  std::vector<const ServerConfig*> server_configs_;

  std::string client_data_;
  std::stringstream raw_response_data_;
  bool keep_alive_flag_;
  Request request_;
  Response response_;

  CgiRequest* cgi_request_;
  std::string cgi_response_message_;

  connection::State httpHandlerRecv(void);
  connection::State httpHandlerSend(void);
  // TODO connection::State httpHandlerCgi(void);

  void prepareToSendResponse(void);

  Http(void);
  Http(const Http&);
  void operator=(const Http&);
};

#endif
