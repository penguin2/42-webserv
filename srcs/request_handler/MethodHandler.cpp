#include <unistd.h>

#include "Connection.hpp"
#include "FileUtils.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "config/ConfigAdapter.hpp"

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
  const std::vector<std::string>& paths = ConfigAdapter::makeAbsolutePaths(
      uri.getHost(), uri.getPort(), uri.getPath());

  return connection::SEND;
  (void)paths;
  (void)request;
  (void)response;
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

connection::State RequestHandler::MethodHandler::unknownMethodHandler(
    const Request& request, Response& response) {
  (void)request;
  (void)response;
  throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                        "Unknown Method");
}
