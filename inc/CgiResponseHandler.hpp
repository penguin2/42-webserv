#ifndef WEBSERV_CGIRESPONSEHANDLER_H_
#define WEBSERV_CGIRESPONSEHANDLER_H_

#include "ConnectionState.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ResponseData.hpp"

namespace CgiResponseHandler {
connection::State convertCgiResponseDataToHttpResponseData(
    const Request& request, Response& response);
connection::State documentResponseHandler(ResponseData& data);
connection::State localRedirectResponseHandler(Response& response,
                                               const Request& const_request);
connection::State clientRedirectResponseHandler(ResponseData& data);
connection::State clientRedirectResponseWithDocumentHandler(ResponseData& data);

namespace INTERNAL {
void convertStatusHeaderToStatusLine(ResponseData& data);
std::string convertLocalLocationToAbsoluteLocation(ResponseData& data,
                                                   const Request& request);

bool isDocumentResponse(const ResponseData& data);
bool isLocalRedirectResponse(const ResponseData& data);
bool isClientRedirectResponse(const ResponseData& data);
bool isClientRedirectResponseWithDocument(const ResponseData& data);
}  // namespace INTERNAL
}  // namespace CgiResponseHandler

#endif
