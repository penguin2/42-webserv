#ifndef WEBSERV_REQUEST_H_
#define WEBSERV_REQUEST_H_

#include <map>
#include <string>

#include "RequestData.hpp"

class Request {
 public:
  Request(void);
  ~Request(void);

  bool parse(std::string& buffer);
  const RequestData* getRequestData(void) const;

 private:
  typedef enum eRequestState {
    START,
    METHOD,
    BETWEEN_METHOD_AND_URI,
    URI,
    BETWEEN_URI_AND_VERSION,
    VERSION,
    BETWEEN_VERSION_AND_HEADER,
    HEADER,
    BODY,
    CHUNKEDBODY,
    END,
  } RequestState;

  // data_ is repeatedly new and delete for each request.
  RequestData* data_;
  RequestState state_;

  void parseMethod(std::string& buffer);
  void parseUri(std::string& buffer);
  void parseVersion(std::string& buffer);
  void parseHeader(std::string& buffer);
  void parseBody(std::string& buffer);
  void parseChunkedBody(std::string& buffer);
  void parseBlank(std::string& buffer);
  void determineParseBody(std::string& buffer);

  Request(const Request&);
  void operator=(const Request&);
};

#endif
