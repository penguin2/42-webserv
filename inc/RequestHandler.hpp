#ifndef WEBSERV_REQUESTHANDLER_H_
#define WEBSERV_REQUESTHANDLER_H_

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace RequestHandler {
connection::State dispatch(const Request& request, Response& response);
connection::State errorRequestHandler(const Request& request,
                                      Response& response, int status_code,
                                      const std::string& phrase);
connection::State redirectHandler(const Request& request, Response& response);
// connection::State cgiHandler(Request& request, Response& response);

connection::State getMethodHandler(const Request& request, Response& response);
connection::State postMethodHandler(const Request& request, Response& response);
connection::State deleteMethodHandler(const Request& request,
                                      Response& response);
connection::State unknownMethodHandler(const Request& request,
                                       Response& response);
}  // namespace RequestHandler

#endif
