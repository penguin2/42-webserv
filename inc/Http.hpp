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

  connection::State httpHandler(void);
  void appendClientData(const std::string& data);
  std::string getResponse(void) const;

  CgiRequest* getCgiRequest() const;
  void setCgiResponseMessage(const std::string& message);

 private:
  typedef enum eHttpState { RECV, SEND } HttpState;
  HttpState state_;
  std::string client_data_;
  std::stringstream raw_response_data_;
  bool keep_alive_flag_;
  Request request_;
  Response response_;

  CgiRequest* cgi_request_;
  std::string cgi_response_message_;

  bool haveConnectionCloseHeader(void) const;
  void insertCommonHeaders(bool keep_alive);

  // TODO CGI以外のWebServer機能の実装
  connection::State dispatchRequestHandler(void);
  connection::State errorContentHandler(int status_code,
                                        const std::string& phrase);
  connection::State redirectHandler(const std::string& redirect_uri);
  // connection::State staticContentHandler(void);

  // TODO CGIを処理する機能
  // connection::State cgiHandler(void);

  Http(const Http&);
  void operator=(const Http&);
};

#endif
