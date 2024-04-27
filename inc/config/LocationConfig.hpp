#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <string>
#include <vector>
#include <iostream>

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
  LocationConfig();

  const std::string& getRoot() const;

  const std::string& getIndex() const;

  const std::string& getTryFilesPath() const;

  int getTryFilesErrorCode() const;

  const std::string& getClientMaxBodySize() const;

  bool getAutoindex() const;

  const std::vector<HttpMethod>& getLimitExceptMethods() const;

  const std::string& getReturnStatusCode() const;

  const std::string& getReturnUri() const;

  const std::string& getCgiPath() const;

  const std::string& getCgiExt() const;

  void print();
};

#endif
