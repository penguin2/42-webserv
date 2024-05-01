#ifndef WEBSERV_HTTP_H_
#define WEBSERV_HTTP_H_

#include <sstream>
#include <string>

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"

// TODO: CgiRequestMock -> CgiRequest
// #include "CgiRequest.hpp"
#include "CgiRequestMock.hpp"

class Http {
 public:
  Http(void);
  ~Http(void);

  connection::State httpHandler(connection::State state);
  void appendClientData(const std::string& data);
  std::string getResponse(void) const;

  CgiRequest* getCgiRequest() const;
  void setCgiResponseMessage(const std::string& message);

 private:
  std::string client_data_;
  std::stringstream raw_response_data_;
  bool keep_alive_flag_;
  Request request_;
  Response response_;

  CgiRequest* cgi_request_;
  std::string cgi_response_message_;

  connection::State callRequestHandler(int status_code,
                                       const std::string& phrase);
  connection::State callRequestHandler(void);

  Http(const Http&);
  void operator=(const Http&);
};

#endif
