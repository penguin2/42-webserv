#include "CgiResponseHandler.hpp"

#include <sstream>

#include "HttpUtils.hpp"
#include "Request.hpp"
#include "ResponseData.hpp"
#include "ServerException.hpp"
#include "Uri.hpp"
#include "Utils.hpp"
#include "config/ConfigAdapter.hpp"

void CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
    const Request& request, ResponseData& data) {
  const std::map<std::string, std::string>& headers = data.getHeaders();
  const std::string& body = data.getBody();
  std::map<std::string, std::string>::const_iterator location =
      headers.find("location");
  std::map<std::string, std::string>::const_iterator status =
      headers.find("status");
  std::map<std::string, std::string>::const_iterator content_type =
      headers.find("content-type");

  if (body.empty() && headers.size() == 1 && location != headers.end()) {
    if (Utils::isStartsWith(location->second, "/"))
      localRedirectResponseHandler(data, request);
    else
      clientRedirectResponseHandler(data, request);
  } else if (!body.empty() && content_type != headers.end()) {
    if (status != headers.end() && location != headers.end() &&
        !Utils::isStartsWith(location->second, "/"))
      clientRedirectResponseWithDocumentHandler(data);
    else
      documentResponseHandler(data);
  } else {
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "INTERNAL Server Error");
  }
}

void CgiResponseHandler::localRedirectResponseHandler(ResponseData& data,
                                                      const Request& request) {
  const size_t redirect_status_code = 302;
  const std::string absolute_location =
      INTERNAL::convertLocalLocationToAbsoluteLocation(data, request);

  const std::string* error_page = ConfigAdapter::searchErrorPage(
      *request.getServerConfig(), redirect_status_code);
  data.appendBody(HttpUtils::generateErrorPage(error_page, redirect_status_code,
                                               "Redirect"));
  data.insertHeader("location", absolute_location);
  data.setStatusCode(redirect_status_code);
  data.setPhrase("Found");
}

void CgiResponseHandler::clientRedirectResponseHandler(ResponseData& data,
                                                       const Request& request) {
  const size_t redirect_status_code = 302;

  const std::string* error_page = ConfigAdapter::searchErrorPage(
      *request.getServerConfig(), redirect_status_code);
  data.appendBody(HttpUtils::generateErrorPage(error_page, redirect_status_code,
                                               "Redirect"));
  data.setStatusCode(redirect_status_code);
  data.setPhrase("Found");
}

void CgiResponseHandler::documentResponseHandler(ResponseData& data) {
  const std::map<std::string, std::string>& headers = data.getHeaders();

  if (headers.find("status") != headers.end()) {
    INTERNAL::convertStatusHeaderToStatusLine(data);
  } else {
    data.setStatusCode(200);
    data.setPhrase("OK");
  }
}

void CgiResponseHandler::clientRedirectResponseWithDocumentHandler(
    ResponseData& data) {
  const std::map<std::string, std::string>& headers = data.getHeaders();

  if (headers.find("status") != headers.end()) {
    INTERNAL::convertStatusHeaderToStatusLine(data);
    if (data.getStatusCode() != 302)
      throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                            "INTERNAL Server Error");
  }
}

void CgiResponseHandler::INTERNAL::convertStatusHeaderToStatusLine(
    ResponseData& data) {
  const std::map<std::string, std::string>::const_iterator status =
      data.getHeaders().find("status");
  const size_t pos_space = status->second.find(' ');
  const std::string code_str = status->second.substr(0, pos_space);
  size_t code;

  Utils::strToSize_t(code_str, code, 10);
  data.setStatusCode(code);
  data.setPhrase(status->second.substr(pos_space));
  data.eraseHeader("status");
}

std::string
CgiResponseHandler::INTERNAL::convertLocalLocationToAbsoluteLocation(
    ResponseData& data, const Request& request) {
  const std::map<std::string, std::string>::const_iterator location =
      data.getHeaders().find("location");

  Uri absolute_location;
  absolute_location.parse(location->second);
  absolute_location.overwriteAuthorityIfNotSet(
      request.getRequestData()->getUri().buildAuthority());
  return absolute_location.buildAbsoluteUri();
}
