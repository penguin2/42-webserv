#ifndef HTTP_EXCEPTION_HPP
#define HTTP_EXCEPTION_HPP

#include <stdexcept>

class HttpException : public std::exception {
 public:
  enum ErrorCode {
    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    CONFLICT = 409,
    PAYLOAD_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    MISDIRECTED_REQUEST = 421,
    HEADER_TOO_LARGE = 431,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    HTTP_VERSION_NOT_SUPPORTED = 505
  };

  HttpException(ErrorCode code, const char* message)
      : m_code(code), m_message(message) {}

  virtual const char* what() const throw() { return m_message; }

  ErrorCode code() const throw() { return m_code; }

 private:
  ErrorCode m_code;
  const char* m_message;
};

#endif
