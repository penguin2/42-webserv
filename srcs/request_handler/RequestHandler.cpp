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
  if (method == "GET")
    return RequestHandler::getMethodHandler(request, response);
  else if (method == "POST")
    return RequestHandler::postMethodHandler(request, response);
  else if (method == "DELETE")
    return RequestHandler::deleteMethodHandler(request, response);
  else
    return RequestHandler::unknownMethodHandler(request, response);
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

// (仮)
connection::State RequestHandler::getMethodHandler(const Request& request,
                                                   Response& response) {
  const Uri& uri = request.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  if (!FileUtils::isExistFile(uri.getPath()))
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND,
                          "File not Found");
  if (!FileUtils::hasFilePermission(uri.getPath(), R_OK))
    throw ServerException(ServerException::SERVER_ERROR_FORBIDDEN,
                          "Has not permission");
  std::stringstream ss;
  if (FileUtils::readAllDataFromFile(uri.getPath(), ss) == false)
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Internal Server Error");
  response.appendBody(ss.str());
  response.insertContentLengthIfNotSet();
  response.insertHeader("Content-Type",
                        HttpUtils::convertPathToContentType(uri.getPath()));
  response.setStatusLine(200, "OK");
  return connection::SEND;
  (void)paths;
}

// (仮)
connection::State RequestHandler::postMethodHandler(const Request& request,
                                                    Response& response) {
  const Uri& uri = request.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  return connection::SEND;
  (void)paths;
  (void)request;
  (void)response;
}

// (仮)
connection::State RequestHandler::deleteMethodHandler(const Request& request,
                                                      Response& response) {
  const Uri& uri = request.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  return connection::SEND;
  (void)paths;
  (void)request;
  (void)response;
}

connection::State RequestHandler::unknownMethodHandler(const Request& request,
                                                       Response& response) {
  (void)request;
  (void)response;
  throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                        "Unknown Method");
}
