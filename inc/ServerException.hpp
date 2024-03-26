#ifndef SERVER_EXCEPTION_HPP
#define SERVER_EXCEPTION_HPP

#include <stdexcept>

class ServerException : public std::exception {
 public:
  enum ErrorCode {
    SERVER_ERROR_NOT_FOUND = 404,
    SERVER_ERROR_FORBIDDEN = 403,
    SERVER_ERROR_INTERNAL_SERVER_ERROR = 500,
    SERVER_ERROR_NOT_IMPLEMENTED = 501,
    SERVER_ERROR_BAD_GATEWAY = 502,
    SERVER_ERROR_SERVICE_UNAVAILABLE = 503,
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
