#include <unistd.h>

#include <utility>

#include "Connection.hpp"
#include "FileUtils.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "config/ConfigAdapter.hpp"

std::map<std::string, RequestHandler::MethodHandler::method_handler>
RequestHandler::MethodHandler::makeMethodHandlerMap(void) {
  std::map<std::string, method_handler> method_handler_map;
  method_handler_map["GET"] = getMethodHandler;
  method_handler_map["POST"] = postMethodHandler;
  method_handler_map["DELETE"] = deleteMethodHandler;
  return method_handler_map;
}

// (仮)
connection::State RequestHandler::MethodHandler::getMethodHandler(
    const Request& request, Response& response) {
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
connection::State RequestHandler::MethodHandler::postMethodHandler(
    const Request& request, Response& response) {
  const Uri& uri = request.getRequestData()->getUri();
  const std::string& body = request.getRequestData()->getBody();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());
  ServerException::ErrorCode return_status_code =
      ServerException::SERVER_ERROR_METHOD_NOT_ALLOWED;

  for (std::vector<std::string>::const_iterator it = paths.begin();
       it != paths.end(); ++it) {
    if (FileUtils::isExistFile(*it)) {
      continue;
    } else if (FileUtils::writeAllDataToFile(*it, body) == false) {
      if (it == paths.begin())
        return_status_code =
            ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR;
    } else {
      response.appendBody(HttpUtils::generatePostSuccessJsonData(*it, uri));
      response.insertContentLengthIfNotSet();
      response.insertHeader("Content-Type", "application/json");
      response.insertHeader("Location", HttpUtils::buildAbsoluteUri(uri));
      response.setStatusLine(201, "Created");
      return connection::SEND;
    }
  }
  throw ServerException(return_status_code, "POST Method Error");
}

// (仮)
connection::State RequestHandler::MethodHandler::deleteMethodHandler(
    const Request& request, Response& response) {
  const Uri& uri = request.getRequestData()->getUri();
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  return connection::SEND;
  (void)paths;
  (void)request;
  (void)response;
}
