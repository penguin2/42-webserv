#ifndef WEBSERV_CGIRESPONSE_H_
#define WEBSERV_CGIRESPONSE_H_

#include "Response.hpp"

class CgiResponse : public Response {
 public:
  CgiResponse(void);
  ~CgiResponse(void);

  bool parse(std::string& buffer);

 private:
  enum CgiResponseState { HEADER, BODY };
  CgiResponseState state_;

  void parseHeader(std::string& buffer);
  void parseBody(std::string& buffer);
  void insertHeaderLine(const std::string& line);
  void insertStatusHeader(const std::string& line);
  void insertLocationHeader(const std::string& value);
  void insertContentTypeHeader(const std::string& value);

  CgiResponse(const CgiResponse&);
  void operator=(const CgiResponse&);
};

#endif
