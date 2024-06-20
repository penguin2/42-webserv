#include <exception>
#include <fstream>
#include <iostream>
#include <map>

#include "HttpException.hpp"
#include "Request.hpp"
#include "config/ServerConfig.hpp"
#include "utils/Utils.hpp"

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

// 0 -> 異常なリクエストで例外をthrow, expect_status_codeと例外のcode()が異なる
// 1 -> 異常なリクエストで例外をthrow, expect_status_codeと例外のcode()が一致
// 2 -> リクエストが未完成
// 3 -> 引数が足りていない
// 4 -> 正常なリクエスト
int main(int argc, char** argv) {
  if (argc < 3) {
    return 3;
  }

  ServerConfig server_config;
  server_config.addLocationConfig("");
  std::vector<const ServerConfig*> server_configs;
  server_configs.push_back(&server_config);
  Request request(server_configs);
  std::string buffer;
  std::string raw_data = getAllChars(argv[1]);

  size_t expect_status_code;
  utils::strToSize_t(argv[2], expect_status_code, 10);

  try {
    for (std::string::const_iterator it = raw_data.begin();
         it != raw_data.end(); ++it) {
      buffer += *it;
      if (request.parse(buffer) == true) return 4;
    }
  } catch (HttpException& e) {
    std::cout << e.code() << e.what() << std::endl;
    return e.code() == expect_status_code;
  }
  return 2;
}
