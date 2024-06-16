#include <unistd.h>

#include <cstdio>
#include <utility>

#include "Connection.hpp"
#include "utils/FileUtils.hpp"
#include "utils/HttpUtils.hpp"
#include "RequestHandler.hpp"
#include "ServerException.hpp"
#include "utils/Utils.hpp"
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

connection::State RequestHandler::MethodHandler::getMethodHandler(
    Request& request, Response& response, const std::string& http_path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, request.getServerConfig()->getLocationConfigs());
  const std::string file_path =
      ConfigAdapter::makeFilePath(*location_conf, http_path);

  if (file_utils::isExistFile(file_path) &&
      file_utils::hasFilePermission(file_path, R_OK)) {
    return getMethodFileHandler(request, response, http_path);
  }
  if (file_utils::isExistDir(file_path) &&
      file_utils::hasFilePermission(file_path, R_OK)) {
    return getMethodDirHandler(request, response, http_path);
  }
  throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not found");
}

connection::State RequestHandler::MethodHandler::getMethodFileHandler(
    Request& request, Response& response, const std::string& http_path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, request.getServerConfig()->getLocationConfigs());
  const std::string file_path =
      ConfigAdapter::makeFilePath(*location_conf, http_path);
  std::stringstream ss;

  if (file_utils::readAllDataFromFile(file_path, ss) == false) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Read Error");
  }
  response.appendBody(ss.str());
  response.insertHeader("Content-Type",
                        http_utils::convertPathToContentType(http_path));
  response.setStatusLine(200, "OK");
  return connection::SEND;
}

connection::State RequestHandler::MethodHandler::getMethodDirHandler(
    Request& request, Response& response, const std::string& http_path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, request.getServerConfig()->getLocationConfigs());
  const std::string file_path =
      ConfigAdapter::makeFilePath(*location_conf, http_path);
  std::stringstream ss;

  const std::string& index = ConfigAdapter::searchIndex(*location_conf);
  if (!index.empty()) {
    try {
      return RequestHandler::dispatch(request, response,
                                      utils::concatWithSlash(http_path, index));
    } catch (ServerException& e) {
      // 内部リダイレクト失敗時の情報は無視, 後工程の処理を続ける
    }
  }
  if (!ConfigAdapter::isAutoindex(*location_conf)) {
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not Found");
  }
  if (http_utils::generateAutoindexPage(http_path, file_path, ss) == false) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Autoindex Error");
  }
  response.appendBody(ss.str());
  response.insertHeader("Content-Type", "text/html");
  response.setStatusLine(200, "OK");
  return connection::SEND;
}

connection::State RequestHandler::MethodHandler::postMethodHandler(
    Request& request, Response& response, const std::string& http_path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, request.getServerConfig()->getLocationConfigs());
  const std::string file_path =
      ConfigAdapter::makeFilePath(*location_conf, http_path);
  const std::string& body = request.getRequestData()->getBody();

  if (!ConfigAdapter::canUpload(*location_conf)) {
    throw ServerException(ServerException::SERVER_ERROR_FORBIDDEN, "Forbidden");
  }
  if (file_utils::isExistFile(file_path) || file_utils::isExistDir(file_path)) {
    throw ServerException(ServerException::SERVER_ERROR_CONFLICT, "Conflict");
  }
  if (ConfigAdapter::getClientMaxBodySize(*location_conf) < body.size()) {
    throw ServerException(ServerException::SERVER_ERROR_PAYLOAD_TOO_LARGE,
                          "Payload too large");
  }
  if (file_utils::writeAllDataToFile(file_path, body) == false) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Internal Error");
  }
  const std::string absolute_uri =
      request.getRequestData()->getUri().buildAbsoluteUri();
  response.appendBody(generatePostSuccessJsonData(file_path, absolute_uri));
  response.insertHeader("Content-Type", "application/json");
  response.insertHeader("Location", absolute_uri);
  response.setStatusLine(201, "Created");
  return connection::SEND;
}

connection::State RequestHandler::MethodHandler::deleteMethodHandler(
    Request& request, Response& response, const std::string& http_path) {
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, request.getServerConfig()->getLocationConfigs());
  const std::string file_path =
      ConfigAdapter::makeFilePath(*location_conf, http_path);

  if (!file_utils::isExistFile(file_path)) {
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not Found");
  }
  if (!file_utils::hasFilePermission(file_path, W_OK)) {
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not Found");
  }
  if (std::remove(file_path.c_str()) != 0) {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Can not remove file");
  }
  response.setStatusLine(204, "No Content");
  return connection::SEND;
}

std::string RequestHandler::MethodHandler::generatePostSuccessJsonData(
    const std::string& file_path, const std::string& absolute_uri) {
  std::stringstream ss;
  const off_t file_size = file_utils::getFileSize(file_path);
  std::time_t raw_time;
  std::time(&raw_time);

  ss << "{\r\n";
  ss << "  \"URI\": " << absolute_uri << ",\r\n";
  if (0 <= file_size) {
    ss << "  \"FILE_SIZE\": " << file_size << ",\r\n";
  }
  ss << "  \"CREATED\": "
     << http_utils::generateDateAsFormat(raw_time, "%Y-%m-%d %H:%M:%S")
     << "\r\n";
  ss << "}\r\n";
  return ss.str();
}
