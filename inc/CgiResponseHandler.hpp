#ifndef WEBSERV_CGIRESPONSEHANDLER_H_
#define WEBSERV_CGIRESPONSEHANDLER_H_

#include "Request.hpp"
#include "ResponseData.hpp"
#include "Uri.hpp"

namespace CgiResponseHandler {
void convertCgiResponseDataToHttpResponseData(const Request& request,
                                              ResponseData& data);
void documentResponseHandler(ResponseData& data);
void localRedirectResponseHandler(ResponseData& data, const Request& request);
void clientRedirectResponseHandler(ResponseData& data, const Request& request);
void clientRedirectResponseWithDocumentHandler(ResponseData& data);

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
