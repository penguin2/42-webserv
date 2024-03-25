#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <string>
#include <vector>

enum HttpMethod { GET, POST, HEAD, PUT, DELETE, OPTIONS, TRACE, CONNECT };

class LocationConfig {
 private:
  std::string root;
  std::string index;
  std::string try_files_path;
  int try_files_error_code;
  std::string client_max_body_size;
  bool autoindex;
  std::vector<HttpMethod> limit_except_methods;
  std::string return_status_code;
  std::string return_uri;
  std::string cgi_path;
  std::string cgi_ext;

 public:
  LocationConfig()
      : root(""),
        index(""),
        try_files_path(""),
        try_files_error_code(0),
        client_max_body_size(""),
        autoindex(false),
        return_status_code(""),
        return_uri(""),
        cgi_path(""),
        cgi_ext("") {}

  const std::string& getRoot() const { return root; }

  const std::string& getIndex() const { return index; }

  const std::string& getTryFilesPath() const { return try_files_path; }

  int getTryFilesErrorCode() const { return try_files_error_code; }

  const std::string& getClientMaxBodySize() const {
    return client_max_body_size;
  }

  bool getAutoindex() const { return autoindex; }

  const std::vector<HttpMethod>& getLimitExceptMethods() const {
    return limit_except_methods;
  }

  const std::string& getReturnStatusCode() const { return return_status_code; }

  const std::string& getReturnUri() const { return return_uri; }

  const std::string& getCgiPath() const { return cgi_path; }

  const std::string& getCgiExt() const { return cgi_ext; }
};

#endif
