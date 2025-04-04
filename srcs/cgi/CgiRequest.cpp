#include "CgiRequest.hpp"

#include <algorithm>
#include <cstring>

#include "Request.hpp"
#include "SocketAddress.hpp"
#include "Time.hpp"
#include "utils/Utils.hpp"
#include "config/ConfigAdapter.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"

CgiRequest::CgiRequest() {}

CgiRequest::~CgiRequest() {}

const std::map<std::string, std::string>& CgiRequest::getEnvVars() const {
  return env_vars_;
}

std::string CgiRequest::getMessage() const { return message_; }

std::string CgiRequest::getDirectory() const { return directory_; }

std::string CgiRequest::getFilename() const { return filename_; }

void CgiRequest::addEnvVar(const std::string& key, const std::string& value) {
  env_vars_[key] = value;
}

void CgiRequest::setMessage(const std::string& message) { message_ = message; }

void CgiRequest::setDirectory(std::string directory) { directory_ = directory; }

void CgiRequest::setFilename(std::string filename) { filename_ = filename; }

CgiRequest* CgiRequest::createCgiRequest(const Request& request,
                                         SocketAddress peer_addr) {
  CgiRequest* new_cgi_request = new CgiRequest();

  const RequestData* data = request.getRequestData();
  const Uri& uri = data->getUri();
  const std::string& http_path = uri.getPath();
  const ServerConfig* server_conf = request.getServerConfig();
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, server_conf->getLocationConfigs());
  const std::string file_path =
      ConfigAdapter::makeFilePath(*location_conf, http_path);
  std::map<std::string, std::string> file_data_map =
      ConfigAdapter::makeFileDataMap(*location_conf, file_path);

  new_cgi_request->insertContentLengthAndBodyIfHasHeader(request);
  new_cgi_request->insertContentTypeIfHasHeader(request);
  new_cgi_request->addEnvVar("GATEWAY_INTERFACE", "CGI/1.1");
  if (!file_data_map["PATH_INFO"].empty()) {
    const std::string path_info = file_data_map["PATH_INFO"];
    const std::string path_translated =
        ConfigAdapter::makeFilePath(*location_conf, path_info);
    new_cgi_request->addEnvVar("PATH_INFO", path_info);
    new_cgi_request->addEnvVar("PATH_TRANSLATED", path_translated);
  }
  new_cgi_request->addEnvVar("QUERY_STRING", uri.getQuery());
  new_cgi_request->addEnvVar("REMOTE_ADDR", peer_addr.getIpAddr());
  new_cgi_request->addEnvVar("REMOTE_HOST", peer_addr.getIpAddr());
  new_cgi_request->addEnvVar("REQUEST_METHOD", data->getMethod());
  new_cgi_request->insertScriptName(request);
  new_cgi_request->insertServerName(request);
  new_cgi_request->addEnvVar("SERVER_PORT",
                             ConfigAdapter::getListenPort(*server_conf));
  new_cgi_request->addEnvVar("SERVER_PROTOCOL", data->getVersion());
  new_cgi_request->addEnvVar("SERVER_SOFTWARE", "Webserv");
  new_cgi_request->insertHTTPHeaders(request);

  new_cgi_request->setDirectory(file_data_map["DIR"]);
  new_cgi_request->setFilename(file_data_map["FILE"]);

  return new_cgi_request;
}

void CgiRequest::insertContentLengthAndBodyIfHasHeader(const Request& request) {
  const std::map<std::string, std::string>& headers =
      request.getRequestData()->getHeaders();
  const std::map<std::string, std::string>::const_iterator content_length =
      headers.find("content-length");

  if (content_length != headers.end()) {
    size_t size;
    if (utils::strToSize_t(content_length->second, size, 10) == false ||
        size == 0)
      return;
    addEnvVar("CONTENT_LENGTH", utils::uintToString(size));
    setMessage(request.getRequestData()->getBody());
  }
}

void CgiRequest::insertContentTypeIfHasHeader(const Request& request) {
  const std::map<std::string, std::string>& headers =
      request.getRequestData()->getHeaders();
  const std::map<std::string, std::string>::const_iterator content_type =
      headers.find("content-type");

  if (content_type != headers.end()) {
    addEnvVar("CONTENT_TYPE", content_type->second);
  }
}

void CgiRequest::insertServerName(const Request& request) {
  const ServerConfig* server_conf = request.getServerConfig();
  const std::string* host_name = ConfigAdapter::searchHostName(*server_conf);

  if (host_name == NULL) {
    addEnvVar("SERVER_NAME", "");
  } else {
    addEnvVar("SERVER_NAME", *host_name);
  }
}

void CgiRequest::insertScriptName(const Request& request) {
  const std::string& http_path = request.getRequestData()->getUri().getPath();
  const LocationConfig* location_conf = ConfigAdapter::searchLocationConfig(
      http_path, request.getServerConfig()->getLocationConfigs());
  std::map<std::string, std::string> file_data_map =
      ConfigAdapter::makeFileDataMap(*location_conf, http_path);

  addEnvVar("SCRIPT_NAME", file_data_map["DIR"] + file_data_map["FILE"]);
}

void CgiRequest::insertHTTPHeaders(const Request& request) {
  const std::map<std::string, std::string>& headers =
      request.getRequestData()->getHeaders();

  for (std::map<std::string, std::string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    std::string cgi_key = "HTTP_" + utils::toUpper(it->first);
    std::replace(cgi_key.begin(), cgi_key.end(), '-', '_');
    addEnvVar(cgi_key, it->second);
  }
}
