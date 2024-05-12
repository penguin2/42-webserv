#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>

enum HttpMethod { GET, POST, DELETE};

class LocationConfig {
 private:
  std::string root;
  std::string index;
  std::vector<std::string> try_files_path;
  int try_files_error_code;
  std::string client_max_body_size;
  bool autoindex;
  std::vector<std::string> allow_methods;
  int return_status_code;
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

  int getTryFilesErrorCode() const;
  void setTryFilesErrorCode(int newErrorCode);

  const std::string& getClientMaxBodySize() const;
  void setClientMaxBodySize(const std::string& newSize);

  bool getAutoindex() const;
  void setAutoindex(bool newAutoindex);

  const std::vector<std::string>& getAllowMethods() const;
  void setAllowMethods(const std::vector<std::string> new_methods);

  int getReturnStatusCode() const;
  void setReturnStatusCode(int newStatusCode);

  const std::string& getReturnUri() const;
  void setReturnUri(const std::string& newUri);

  const std::vector<std::string>& getCgiPath() const;
  void addCgiPath(const std::string& newPath);

  const std::vector<std::string>& getCgiExt() const;
  void addCgiExt(const std::string& newExt);

  void print();
};

#endif
