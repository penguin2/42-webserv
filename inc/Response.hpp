#ifndef WEBSERV_RESPONSE_H_
#define WEBSERV_RESPONSE_H_

#include <sstream>
#include <string>

#include "ResponseData.hpp"

class Response {
 public:
  Response(void);
  ~Response(void);

  void getResponseRawData(std::stringstream &ss) const;

  void setStatusLine(int code, const std::string &phrase);
  void insertHeader(const std::string &key, const std::string &value);
  void appendBody(const std::string &str);
  void resetResponseData(void);
  void insertContentLengthIfNotSet(void);

  // TODO CGI用インターフェースの作成
  // bool cgiParse(std::stringstream&);

 private:
  // CGIパース時に使用するステート
  // typedef enum eResponseState { HEADER, BODY } ResponseState;
  // ResponseState state_;

  ResponseData *data_;

  Response(const Response &);
  void operator=(const Response &);
};

#endif
