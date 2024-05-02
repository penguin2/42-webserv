#include "RequestHandler.hpp"

#include <unistd.h>

#include "ConnectionState.hpp"
#include "HttpUtils.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"

connection::State RequestHandler::dispatch(const Request& request,
                                           Response& response) {
  static const std::map<std::string, MethodHandler::method_handler>
      method_handler_map = MethodHandler::makeMethodHandlerMap();
  const Uri& uri = request.getRequestData()->getUri();

  if (!ConfigAdapter::isAllowMethods(uri.getHost(), uri.getPort(),
                                     uri.getPath(),
                                     request.getRequestData()->getMethod()))
    throw ServerException(ServerException::SERVER_ERROR_METHOD_NOT_ALLOWED,
                          "Method not allowed");

  const std::string* redirect_uri = ConfigAdapter::searchRedirectUri(
      uri.getHost(), uri.getPort(), uri.getPath());
  if (redirect_uri != NULL) {
    return RequestHandler::redirectHandler(request, response);
  }
  // TODO CGI Handle
  // else if (ConfigAdapter::isCgiPath(uri.getHost(), uri.getPort(),
  //   		  uri.getPath())) {
  //     return cgiHandler();
  // }
  const std::string& method = request.getRequestData()->getMethod();
  if (method_handler_map.find(method) != method_handler_map.end())
    return method_handler_map.find(method)->second(request, response);
  throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                        "Unknown Method");
}

connection::State RequestHandler::redirectHandler(const Request& request,
                                                  Response& response) {
  const Uri& uri = request.getRequestData()->getUri();
  // const std::string* redirect_uri = ConfigAdapter::searchRedirectUri(
  //    uri.getHost(), uri.getPort(), uri.getPath());
  const int redirect_status_code = ConfigAdapter::searchRedirectStatusCode(
      uri.getHost(), uri.getPort(), uri.getPath());

  if (!HttpUtils::isRedirectStatusCode(redirect_status_code))
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Return directive is invalid");
  const std::string* error_page = ConfigAdapter::searchErrorPage(
      uri.getHost(), uri.getPort(), redirect_status_code);
  response.appendBody(HttpUtils::generateErrorPage(
      error_page, redirect_status_code, "Redirect"));
  response.insertContentLengthIfNotSet();
  // TODO redirect_uri use config data
  response.insertHeader("Location", "/var/www/html/new-location/index.html");
  response.setStatusLine(redirect_status_code, "Redirect");
  return connection::SEND;
}

connection::State RequestHandler::errorRequestHandler(
    const Request& request, Response& response, int status_code,
    const std::string& phrase) {
  const Uri& uri = request.getRequestData()->getUri();
  const std::string* error_page =
      ConfigAdapter::searchErrorPage(uri.getHost(), uri.getPort(), status_code);

  response.appendBody(
      HttpUtils::generateErrorPage(error_page, status_code, phrase));
  response.insertHeader("Content-Type", "text/html");
  response.insertContentLengthIfNotSet();
  if (status_code == 405) {
    const Uri& uri = request.getRequestData()->getUri();
    const std::vector<std::string> allow_methods =
        ConfigAdapter::getAllowMethods(uri.getHost(), uri.getPort(),
                                       uri.getPath());
    response.insertHeader("Allow", Utils::joinStrings(allow_methods, " ,"));
  }
  response.setStatusLine(status_code, phrase);
  return connection::SEND;
}
