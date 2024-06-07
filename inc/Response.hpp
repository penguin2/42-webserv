#ifndef WEBSERV_RESPONSE_H_
#define WEBSERV_RESPONSE_H_

#include <sstream>
#include <string>

#include "ResponseData.hpp"

class Response {
 public:
  Response(void);
  virtual ~Response(void);

  void getResponseRawData(std::stringstream &ss) const;
  ResponseData &getResponseData(void);

  void setStatusLine(size_t status_code, const std::string &phrase);
  void insertHeader(const std::string &key, const std::string &value);
  void appendBody(const std::string &str);
  void resetResponseData(void);
  void insertCommonHeaders(bool keep_alive);
  size_t getStatusCode(void) const;
  bool insertSetCookieHeader(const std::string &set_cookie_header_value);
  void clearBody(void);

 protected:
  ResponseData *data_;

 private:
  void insertContentLength(void);

  Response(const Response &);
  void operator=(const Response &);
};

#endif
