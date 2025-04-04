#ifndef WEBSERV_REQUEST_H_
#define WEBSERV_REQUEST_H_

#include <map>
#include <string>

#include "RequestData.hpp"
#include "config/ServerConfig.hpp"

class Request {
 public:
  Request(const std::vector<const ServerConfig*>& server_configs);
  ~Request(void);

  bool parse(std::string& buffer);
  const RequestData* getRequestData(void) const;
  bool haveConnectionCloseHeader(void) const;
  void setServerConfig(const ServerConfig& server_conf);
  const ServerConfig* getServerConfig(void) const;
  void overwritePath(const std::string& new_path);
  void overwritePathQuery(const std::string& new_path_query);

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
  size_t header_line_counter_;
  size_t crlf_counter_before_method_;
  std::vector<const ServerConfig*> server_configs_;

  void parseMethod(std::string& buffer);
  void parseUri(std::string& buffer);
  void parseVersion(std::string& buffer);
  void parseHeader(std::string& buffer);
  void parseBody(std::string& buffer);
  void parseChunkedBody(std::string& buffer);
  void parseChunkedSize(std::string& buffer);
  void determineParseBody(std::string& buffer);
  void insertContentLength(void);

  Request(void);
  Request(const Request&);
  void operator=(const Request&);
};

#endif
