#ifndef WEBSERV_HTTP_H_
#define WEBSERV_HTTP_H_

#include <sstream>
#include <string>

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Http {
 public:
  Http(void);
  ~Http(void);

  connection::State httpHandler(void);
  void appendClientData(const std::string& data);
  std::string getResponse(void) const;

 private:
  typedef enum eHttpState { RECV, SEND } HttpState;
  HttpState state_;
  std::string client_data_;
  std::stringstream raw_response_data_;
  bool keep_alive_flag_;
  Request request_;
  Response response_;

  // TODO CGI以外のWebServer機能の実装
  connection::State dispatch(void);
  connection::State errorContentHandler(int status_code,
                                        const std::string& phrase);
  // connection::State redirectHandler(void);
  // connection::State staticContentHandler(void);

  // TODO CGIを処理する機能
  // connection::State cgiHandler(void);

  bool isConnectionKeepAlive(void) const;
  void insertConnectionHeader(bool keep_alive);

  Http(const Http&);
  void operator=(const Http&);
};

#endif
