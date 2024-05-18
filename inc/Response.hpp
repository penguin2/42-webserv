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

  void setStatusLine(int code, const std::string &phrase);
  void insertHeader(const std::string &key, const std::string &value);
  void appendBody(const std::string &str);
  void resetResponseData(void);
  void insertContentLengthIfNotSet(void);
  void insertCommonHeaders(bool keep_alive);
  int getStatusCode(void) const;

 protected:
  ResponseData *data_;

 private:
  Response(const Response &);
  void operator=(const Response &);
};

#endif
