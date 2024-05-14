#include "RequestHandler.hpp"

#include <unistd.h>

#include "ConnectionState.hpp"
#include "HttpUtils.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/LocationConfig.hpp"

connection::State RequestHandler::dispatch(Request& request, Response& response,
                                           std::string path) {
  static const std::map<std::string, MethodHandler::method_handler>
      method_handler_map = MethodHandler::makeMethodHandlerMap();
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      path, request.getServerConfig()->getLocationConfigs());

  if (location_conf == NULL)
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not Found");

  if (!ConfigAdapter::isAllowMethods(*location_conf,
                                     request.getRequestData()->getMethod()))
    throw ServerException(ServerException::SERVER_ERROR_METHOD_NOT_ALLOWED,
                          "Method not allowed");

  if (ConfigAdapter::searchRedirectUri(*location_conf) != NULL) {
    return RequestHandler::redirectHandler(request, response, *location_conf);
  }
  // TODO CGI Handle
  // else if (ConfigAdapter::isCgiPath(uri.getHost(), uri.getPort(),
  //   		  uri.getPath())) {
  //     return cgiHandler();
  // }
  const std::string& method = request.getRequestData()->getMethod();
  if (method_handler_map.find(method) != method_handler_map.end())
    return method_handler_map.find(method)->second(request, response,
                                                   *location_conf);

  throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                        "Unknown Method");
}

connection::State RequestHandler::redirectHandler(
    const Request& request, Response& response,
    const LocationConfig& location_conf) {
  const std::string* redirect_uri =
      ConfigAdapter::searchRedirectUri(location_conf);
  int redirect_status_code =
      ConfigAdapter::searchRedirectStatusCode(location_conf);

  if (!HttpUtils::isRedirectStatusCode(redirect_status_code))
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Return directive is invalid");

  response.appendBody(
      generateErrorPageContent(request, redirect_status_code, "Redirect"));
  response.insertHeader("Location", *redirect_uri);
  response.insertHeader("Content-Type", "text/html");
  response.insertContentLengthIfNotSet();
  response.setStatusLine(redirect_status_code, "Redirect");
  return connection::SEND;
}

connection::State RequestHandler::errorRequestHandler(
    const Request& request, Response& response, int status_code,
    const std::string& phrase) {
  response.appendBody(generateErrorPageContent(request, status_code, phrase));
  response.insertHeader("Content-Type", "text/html");
  response.insertContentLengthIfNotSet();
  response.setStatusLine(status_code, phrase);
  return connection::SEND;
}

std::string RequestHandler::generateErrorPageContent(
    const Request& request, int status_code, const std::string& phrase) {
  if (request.getServerConfig() == NULL)
    return HttpUtils::generateErrorPage(status_code, phrase);
  else {
    const std::string* error_page =
        ConfigAdapter::searchErrorPage(*request.getServerConfig(), status_code);
    return HttpUtils::generateErrorPage(error_page, status_code, phrase);
  }
}
