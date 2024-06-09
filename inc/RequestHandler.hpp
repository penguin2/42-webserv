#ifndef WEBSERV_REQUESTHANDLER_H_
#define WEBSERV_REQUESTHANDLER_H_

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "config/LocationConfig.hpp"

namespace RequestHandler {
connection::State dispatch(Request& request, Response& response,
                           const std::string& http_path);
connection::State errorRequestHandler(const Request& request,
                                      Response& response, size_t status_code,
                                      const std::string& phrase);
connection::State redirectHandler(const Request& request, Response& response,
                                  const std::string& http_path);
std::string generateErrorPageContent(const Request& request, size_t status_code,
                                     const std::string& phrase);
connection::State cgiHandler(Request& request, const std::string& http_path);

namespace MethodHandler {
typedef connection::State (*method_handler)(Request&, Response&,
                                            const std::string& http_path);
std::map<std::string, method_handler> makeMethodHandlerMap(void);

connection::State getMethodHandler(Request& request, Response& response,
                                   const std::string& http_path);
connection::State getMethodFileHandler(Request& request, Response& response,
                                       const std::string& http_path);
connection::State getMethodDirHandler(Request& request, Response& response,
                                      const std::string& http_path);
connection::State postMethodHandler(Request& request, Response& response,
                                    const std::string& http_path);
connection::State deleteMethodHandler(Request& request, Response& response,
                                      const std::string& http_path);
std::string generatePostSuccessJsonData(const std::string& file_path,
                                        const std::string& absolute_uri);
}  // namespace MethodHandler
}  // namespace RequestHandler

#endif
