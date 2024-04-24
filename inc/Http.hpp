#ifndef WEBSERV_HTTP_H_
#define WEBSERV_HTTP_H_

#include <sstream>
#include <string>
#include <vector>

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

  bool haveConnectionCloseHeader(void) const;
  void insertCommonHeaders(bool keep_alive);

  connection::State dispatchRequestHandler(void);
  // RequestHandler
  connection::State errorContentHandler(int status_code,
                                        const std::string& phrase);
  connection::State redirectHandler(const std::string& redirect_uri);
  connection::State staticContentHandler(void);
  // TODO CGIを処理する機能
  // connection::State cgiHandler(void);

  // MethodHandler
  typedef connection::State (Http::*MethodHandler)(
      std::vector<std::string>& paths);
  std::map<std::string, MethodHandler> method_handler_map_;

  connection::State getMethodHandler(std::vector<std::string>& paths);
  connection::State postMethodHandler(std::vector<std::string>& paths);
  connection::State deleteMethodHandler(std::vector<std::string>& paths);

  Http(const Http&);
  void operator=(const Http&);
};

#endif
