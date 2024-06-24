#include <exception>
#include <fstream>
#include <iostream>
#include <map>

#include "HttpException.hpp"
#include "Request.hpp"
#include "config/ServerConfig.hpp"

#define SAMPLE_REQUEST               \
  "\r\nGET /index.html HTTP/1.1\r\n" \
  "Host: a\r\n"                      \
  "Transfer-Encoding: chunked\r\n"   \
  "\r\n"                             \
  "1\r\n"                            \
  "2\r\n"                            \
  "3\r\n"                            \
  "456\r\n"                          \
  "0\r\n"

static void printRequest(const RequestData* request) {
  std::cout << "Method  : " << request->getMethod() << std::endl;
  std::cout << "Uri     : " << request->getUri().getPath() << std::endl;
  std::cout << "Version : " << request->getVersion() << std::endl;
  const std::map<std::string, std::string>& headers = request->getHeaders();
  std::cout << "Headers :";
  for (std::map<std::string, std::string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    std::cout << " \"" << it->first << ": " << it->second << "\"";
  }
  std::cout << std::endl;
  int body_size = request->getBody().size();
  if (body_size != 0)
    std::cout << "Size    : " << body_size << std::endl
              << "Body    : " << request->getBody() << std::endl;
}

static std::string getAllChars(const char* file) {
  std::fstream fs(file);
  std::string all_chars;
  std::string line;

  while (std::getline(fs, line)) {
    all_chars += line;
    if (fs.eof() == false) all_chars += std::string("\r\n");
  }
  return all_chars;
}

// 0 -> 正常なリクエスト
// 1 -> 異常なリクエストで例外をthrow
// 2 -> リクエストが未完成
int main(int argc, char** argv) {
  ServerConfig server_config;
  server_config.addLocationConfig("");
  std::vector<const ServerConfig*> server_configs;
  server_configs.push_back(&server_config);
  Request request(server_configs);
  std::string buffer;
  std::string raw_data = (argc == 1) ? SAMPLE_REQUEST : getAllChars(argv[1]);

  try {
    for (std::string::const_iterator it = raw_data.begin();
         it != raw_data.end(); ++it) {
      buffer += *it;
      if (request.parse(buffer) == true) {
        printRequest(request.getRequestData());
        return 0;
      }
    }
  } catch (HttpException& e) {
    std::cout << e.code() << e.what() << std::endl;
    return 1;
  }
  return 2;
}
