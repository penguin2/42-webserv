#include "CgiResponseHandler.hpp"

#include "ConnectionState.hpp"
#include "HttpException.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"
#include "ResponseData.hpp"
#include "Uri.hpp"
#include "utils/HttpUtils.hpp"
#include "utils/Utils.hpp"

connection::State CgiResponseHandler::convertCgiResponseDataToHttpResponseData(
    const Request& request, Response& response) {
  ResponseData& data = response.getResponseData();
  if (INTERNAL::isDocumentResponse(data))
    return documentResponseHandler(data);
  else if (INTERNAL::isLocalRedirectResponse(data))
    return localRedirectResponseHandler(response, request);
  else if (INTERNAL::isClientRedirectResponse(data))
    return clientRedirectResponseHandler(data);
  else if (INTERNAL::isClientRedirectResponseWithDocument(data))
    return clientRedirectResponseWithDocumentHandler(data);
  else
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Internal Server Error");
}

connection::State CgiResponseHandler::documentResponseHandler(
    ResponseData& data) {
  const std::map<std::string, std::string>& headers = data.getHeaders();

  if (headers.find("status") != headers.end()) {
    // 3digitであれば全て許容する仕様
    INTERNAL::convertStatusHeaderToStatusLine(data);
  } else {
    data.setStatusCode(200);
    data.setPhrase("OK");
  }
  return connection::SEND;
}

connection::State CgiResponseHandler::localRedirectResponseHandler(
    Response& response, const Request& const_request) {
  Request& request = const_cast<Request&>(const_request);
  const std::map<std::string, std::string>& headers =
      response.getResponseData().getHeaders();
  const std::map<std::string, std::string>::const_iterator location =
      headers.find("location");
  request.overwritePathQuery(location->second);
  const std::string http_path = request.getRequestData()->getUri().getPath();

  response.resetResponseData();
  return RequestHandler::dispatch(request, response, http_path);
}

connection::State CgiResponseHandler::clientRedirectResponseHandler(
    ResponseData& data) {
  const size_t redirect_status_code = 302;
  const std::map<std::string, std::string>& headers = data.getHeaders();
  const std::string& absolute_location = headers.find("location")->second;

  data.appendBody(http_utils::generateRedirectContent(
      absolute_location, redirect_status_code, "Found"));
  data.setStatusCode(redirect_status_code);
  data.setPhrase("Found");
  return connection::SEND;
}

connection::State CgiResponseHandler::clientRedirectResponseWithDocumentHandler(
    ResponseData& data) {
  INTERNAL::convertStatusHeaderToStatusLine(data);
  if (!http_utils::isRedirectStatusCode(data.getStatusCode()))
    throw HttpException(HttpException::INTERNAL_SERVER_ERROR,
                        "Internal Server Error");
  return connection::SEND;
}

void CgiResponseHandler::INTERNAL::convertStatusHeaderToStatusLine(
    ResponseData& data) {
  const std::map<std::string, std::string>::const_iterator status =
      data.getHeaders().find("status");
  const size_t pos_space = status->second.find(' ');
  const std::string code_str = status->second.substr(0, pos_space);
  size_t code;

  utils::strToSize_t(code_str, code, 10);
  data.setStatusCode(code);
  data.setPhrase(status->second.substr(pos_space + 1));
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

bool CgiResponseHandler::INTERNAL::isDocumentResponse(
    const ResponseData& data) {
  bool has_body = !data.getBody().empty();
  const std::map<std::string, std::string>& headers = data.getHeaders();
  bool has_content_type = headers.find("content-type") != headers.end();
  bool has_location = headers.find("location") != headers.end();

  return (has_content_type && !has_location && has_body);
}

bool CgiResponseHandler::INTERNAL::isLocalRedirectResponse(
    const ResponseData& data) {
  bool has_body = !data.getBody().empty();
  const std::map<std::string, std::string>& headers = data.getHeaders();
  const std::map<std::string, std::string>::const_iterator location =
      headers.find("location");
  bool only_location = (headers.size() == 1) && (location != headers.end());

  return (only_location && utils::isStartsWith(location->second, "/") &&
          !has_body);
}

bool CgiResponseHandler::INTERNAL::isClientRedirectResponse(
    const ResponseData& data) {
  bool has_body = !data.getBody().empty();
  const std::map<std::string, std::string>& headers = data.getHeaders();
  std::map<std::string, std::string>::const_iterator location =
      headers.find("location");
  bool only_location = (headers.size() == 1) && (location != headers.end());

  return (only_location && !utils::isStartsWith(location->second, "/")) &&
         !has_body;
}

bool CgiResponseHandler::INTERNAL::isClientRedirectResponseWithDocument(
    const ResponseData& data) {
  const std::map<std::string, std::string>& headers = data.getHeaders();
  bool has_body = !data.getBody().empty();
  std::map<std::string, std::string>::const_iterator location =
      headers.find("location");
  bool has_location = location != headers.end();
  bool has_content_type = headers.find("content-type") != headers.end();
  bool has_status = headers.find("status") != headers.end();

  return (has_content_type && has_status && has_location &&
          !utils::isStartsWith(location->second, "/") && has_body);
}
