#ifndef WEBSERV_REQUESTHANDLER_H_
#define WEBSERV_REQUESTHANDLER_H_

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "config/LocationConfig.hpp"

namespace RequestHandler {
connection::State dispatch(Request& request, Response& response,
                           std::string path);
connection::State errorRequestHandler(const Request& request,
                                      Response& response, int status_code,
                                      const std::string& phrase);
connection::State redirectHandler(const Request& request, Response& response,
                                  const LocationConfig& location_conf);
std::string generateErrorPageContent(const Request& request, int status_code,
                                     const std::string& phrase);
// connection::State cgiHandler(Request& request, Response& response);

namespace MethodHandler {
typedef connection::State (*method_handler)(const Request&, Response&,
                                            const LocationConfig&);
std::map<std::string, method_handler> makeMethodHandlerMap(void);

connection::State getMethodHandler(const Request& request, Response& response,
                                   const LocationConfig& location_conf);
connection::State postMethodHandler(const Request& request, Response& response,
                                    const LocationConfig& location_conf);
connection::State deleteMethodHandler(const Request& request,
                                      Response& response,
                                      const LocationConfig& location_conf);
}  // namespace MethodHandler
}  // namespace RequestHandler

#endif
