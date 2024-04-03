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
    METHOD,
    URI,
    VERSION,
    HEADER,
    BODY,
    CHUNKED_BODY,
    CHUNKED_SIZE,
    END
  } RequestState;

  // data_ is repeatedly new and delete for each request.
  RequestData* data_;
  RequestState state_;
  size_t body_size_;

  void parseMethod(std::string& buffer);
  void parseUri(std::string& buffer);
  void parseVersion(std::string& buffer);
  void parseHeader(std::string& buffer);
  void parseBody(std::string& buffer);
  void parseChunkedBody(std::string& buffer);
  void parseChunkedSize(std::string& buffer);
  void determineParseBody(std::string& buffer);
  void insertContentLength(void);

  Request(const Request&);
  void operator=(const Request&);
};

#endif
