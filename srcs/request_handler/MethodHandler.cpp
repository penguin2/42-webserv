#include <unistd.h>

#include <cstdio>
#include <utility>

#include "Connection.hpp"
#include "FileUtils.hpp"
#include "HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/LocationConfig.hpp"

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
    const Request& request, Response& response,
    const LocationConfig& location_conf) {
  const Uri& uri = request.getRequestData()->getUri();

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
}

connection::State RequestHandler::MethodHandler::postMethodHandler(
    const Request& request, Response& response,
    const LocationConfig& location_conf) {
  const std::string& body = request.getRequestData()->getBody();
  std::string absolute_path = ConfigAdapter::makeAbsolutePath(
      location_conf, request.getRequestData()->getUri().getPath());

  if (FileUtils::isExistFile(absolute_path)) {
    throw ServerException(ServerException::SERVER_ERROR_METHOD_NOT_ALLOWED,
                          "File Exist");
  } else if (FileUtils::writeAllDataToFile(absolute_path, body) == false) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Write Error");
  } else {
    const std::string absolute_uri =
        request.getRequestData()->getUri().buildAbsoluteUri();
    response.appendBody(
        generatePostSuccessJsonData(absolute_path, absolute_uri));
    response.insertHeader("Content-Type", "application/json");
    response.insertHeader("Location", absolute_uri);
    response.insertContentLengthIfNotSet();
    response.setStatusLine(201, "Created");
    return connection::SEND;
  }
}

connection::State RequestHandler::MethodHandler::deleteMethodHandler(
    const Request& request, Response& response,
    const LocationConfig& location_conf) {
  std::string absolute_path = ConfigAdapter::makeAbsolutePath(
      location_conf, request.getRequestData()->getUri().getPath());

  if (!FileUtils::isExistFile(absolute_path)) {
    throw ServerException(ServerException::SERVER_ERROR_FORBIDDEN,
                          "File Not Exist");
  } else if (!FileUtils::hasFilePermission(absolute_path, W_OK)) {
    throw ServerException(ServerException::SERVER_ERROR_FORBIDDEN,
                          "Has not permission");
  } else if (std::remove(absolute_path.c_str()) != 0) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Can not remove file");
  } else {
    response.setStatusLine(204, "No Content");
    return connection::SEND;
  }
}

std::string RequestHandler::MethodHandler::generatePostSuccessJsonData(
    const std::string& absolute_path, const std::string& absolute_uri) {
  std::stringstream ss;
  const off_t file_size = FileUtils::getFileSize(absolute_path);
  std::time_t raw_time;
  std::time(&raw_time);

  ss << "{\r\n";
  ss << "  \"URI\": " << absolute_uri << ",\r\n";
  if (0 <= file_size) {
    ss << "  \"FILE_SIZE\": " << file_size << ",\r\n";
  }
  ss << "  \"CREATED\": "
     << HttpUtils::generateDateAsFormat(raw_time, "%Y-%m-%d %H:%M:%S")
     << "\r\n";
  ss << "}\r\n";
  return ss.str();
}
