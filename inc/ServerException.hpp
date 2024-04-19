#ifndef SERVER_EXCEPTION_HPP
#define SERVER_EXCEPTION_HPP

#include <stdexcept>

class ServerException : public std::exception {
 public:
  enum ErrorCode {
    SERVER_ERROR_BAD_REQUEST = 400,
    SERVER_ERROR_FORBIDDEN = 403,
    SERVER_ERROR_NOT_FOUND = 404,
    SERVER_ERROR_METHOD_NOT_ALLOWED = 405,
    SERVER_ERROR_PAYLOAD_TOO_LARGE = 413,
    SERVER_ERROR_URI_TOO_LONG = 414,
    SERVER_ERROR_MISDIRECTED_REQUEST = 421,
    SERVER_ERROR_HEADER_TOO_LARGE = 431,
    SERVER_ERROR_INTERNAL_SERVER_ERROR = 500,
    SERVER_ERROR_NOT_IMPLEMENTED = 501,
    SERVER_ERROR_BAD_GATEWAY = 502,
    SERVER_ERROR_SERVICE_UNAVAILABLE = 503,
    SERVER_ERROR_HTTP_VERSION_NOT_SUPPORTED = 505
  };

  ServerException(ErrorCode code, const char* message)
      : m_code(code), m_message(message) {}

  virtual const char* what() const throw() { return m_message; }

  ErrorCode code() const throw() { return m_code; }

 private:
  ErrorCode m_code;
  const char* m_message;
};

#endif
