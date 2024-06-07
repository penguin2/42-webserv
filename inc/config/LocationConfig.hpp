#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>

enum HttpMethod { GET, POST, DELETE };

class LocationConfig {
 private:
  std::string root;
  std::string index;
  std::vector<std::string> try_files_path;
  size_t try_files_error_code;
  size_t client_max_body_size;
  bool autoindex;
  std::vector<std::string> allow_methods;
  size_t return_status_code;
  std::string return_uri;
  std::vector<std::string> cgi_path;
  std::vector<std::string> cgi_ext;

 public:
  LocationConfig();

  const std::string& getRoot() const;
  void setRoot(const std::string& newRoot);

  const std::string& getIndex() const;
  void setIndex(const std::string& newIndex);

  const std::vector<std::string>& getTryFilesPath() const;
  void addTryFilesPath(const std::string& newPath);

  size_t getTryFilesErrorCode() const;
  void setTryFilesErrorCode(size_t newErrorCode);

  size_t getClientMaxBodySize() const;
  void setClientMaxBodySize(size_t newSize);

  bool getAutoindex() const;
  void setAutoindex(bool newAutoindex);

  const std::vector<std::string>& getAllowMethods() const;
  void setAllowMethods(const std::vector<std::string> new_methods);

  size_t getReturnStatusCode() const;
  void setReturnStatusCode(size_t newStatusCode);

  const std::string& getReturnUri() const;
  void setReturnUri(const std::string& newUri);

  const std::vector<std::string>& getCgiPath() const;
  void addCgiPath(const std::string& newPath);

  const std::vector<std::string>& getCgiExt() const;
  void addCgiExt(const std::string& newExt);

  void print();
};

#endif
