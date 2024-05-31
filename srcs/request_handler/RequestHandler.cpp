#include "RequestHandler.hpp"

#include <unistd.h>

#include "ConnectionState.hpp"
#include "FileUtils.hpp"
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

  // ここでNULLcheckをするので以降searchLocationConfig実行時,NULLが返ることは絶対にない
  if (location_conf == NULL) {
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not Found");
  }
  if (!ConfigAdapter::isAllowMethods(*location_conf,
                                     request.getRequestData()->getMethod())) {
    throw ServerException(ServerException::SERVER_ERROR_METHOD_NOT_ALLOWED,
                          "Method not allowed");
  }
  if (ConfigAdapter::searchRedirectUri(*location_conf) != NULL) {
    return RequestHandler::redirectHandler(request, response, path);
  }
  if (ConfigAdapter::isCgiPath(*location_conf, path)) {
    return cgiHandler(request, path);
  }
  const std::string& method = request.getRequestData()->getMethod();
  if (method_handler_map.find(method) != method_handler_map.end()) {
    return method_handler_map.find(method)->second(request, response, path);
  }
  throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                        "Unknown Method");
}

connection::State RequestHandler::redirectHandler(const Request& request,
                                                  Response& response,
                                                  std::string path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      path, request.getServerConfig()->getLocationConfigs());
  const std::string* redirect_uri =
      ConfigAdapter::searchRedirectUri(*location_conf);
  int redirect_status_code =
      ConfigAdapter::searchRedirectStatusCode(*location_conf);

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
  // status_code == 405 になるのはdispatch関数のsearchLocationConfig関数実行後
  if (status_code == 405) {
    const std::string& path = request.getRequestData()->getUri().getPath();
    // 制御フロー的に必ず (location_conf != NULL) になります
    const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
        path, request.getServerConfig()->getLocationConfigs());
    std::vector<std::string> allow_methods =
        ConfigAdapter::getAllowMethods(*location_conf);
    response.insertHeader("Allow", Utils::joinStrings(allow_methods, ", "));
  }

  response.appendBody(generateErrorPageContent(request, status_code, phrase));
  response.insertHeader("Content-Type", "text/html");
  response.insertContentLengthIfNotSet();
  response.setStatusLine(status_code, phrase);
  return connection::SEND;
}

connection::State RequestHandler::cgiHandler(Request& request,
                                             const std::string& path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      path, request.getServerConfig()->getLocationConfigs());
  std::string full_path = ConfigAdapter::makeAbsolutePath(*location_conf, path);
  std::map<std::string, std::string> file_data_map =
      ConfigAdapter::makeFileDataMap(*location_conf, full_path);
  std::string cgi_file_path = file_data_map["DIR"] + file_data_map["FILE"];

  if (!FileUtils::hasFilePermission(cgi_file_path, X_OK))
    throw ServerException(ServerException::SERVER_ERROR_FORBIDDEN, "Forbidden");

  request.overwritePath(path);
  return connection::CGI;
}

std::string RequestHandler::generateErrorPageContent(
    const Request& request, int status_code, const std::string& phrase) {
  // requestのParseError時はServerConfigが特定できていないためNULL
  const ServerConfig* server_conf = request.getServerConfig();

  if (server_conf == NULL)
    return HttpUtils::generateErrorPage(status_code, phrase);
  else {
    const std::string* error_page =
        ConfigAdapter::searchErrorPage(*server_conf, status_code);
    return HttpUtils::generateErrorPage(error_page, status_code, phrase);
  }
}
