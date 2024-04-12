#include "HttpUtils.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "ServerException.hpp"
#include "Utils.hpp"

// ファイルからデータを読み取り文字列に変換
// file or dir の確認や権限確認はこの関数外で行う想定
std::string HttpUtils::generatePage(const std::string& file) {
  std::ifstream ifs;
  std::stringstream ss;

  const std::string ext(Utils::getExtension(file));
  bool is_binary =
      (ext == "" || ext == "pdf" || ext == "png" || ext == "jpeg" ||
       ext == "gif" || ext == "svg" || ext == "zip" || ext == "mpeg");
  // binary or text
  ifs.open(file.c_str(), ((is_binary) ? (std::ios::binary) : (std::ios::in)));
  if (ifs.fail())
    throw ServerException(ServerException::SERVER_ERROR_NOT_FOUND, "Not Found");
  if (is_binary) {
    ss << ifs.rdbuf();
  } else {
    std::string line;
    while (std::getline(ifs, line)) {
      ss << line;
      if (ifs.eof() == false) ss << "\r\n";
    }
  }
  ifs.close();
  if (ifs.eof() == false && ifs.fail())
    throw ServerException(ServerException::SERVER_ERROR_INTERNAL_SERVER_ERROR,
                          "Internal Server Error");
  return ss.str();
}

// デフォルトのエラーページHTMLをプログラムで生成
std::string HttpUtils::generateErrorPage(int code, const std::string& phrase) {
  std::stringstream ss;

  ss << "<!DOCTYPE html>\r\n"
     << "<html lang=\"en\">\r\n\r\n"
     << "<head>\r\n"
     << "	<meta charset=\"UTF-8\">\r\n"
     << "	<meta name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1.0\">\r\n"
     << "	<title>" << code << " " << phrase << "</title>\r\n"
     << "	<style>\r\n"
     << "		body {\r\n"
     << "			font-family: Arial, sans-serif;\r\n"
     << "			background-color: #f4f4f4;\r\n"
     << "			color: #333;\r\n"
     << "			margin: 0;\r\n"
     << "			padding: 0;\r\n"
     << "			display: flex;\r\n"
     << "			justify-content: center;\r\n"
     << "			align-items: center;\r\n"
     << "			height: 100vh;\r\n"
     << "			text-align: center;\r\n"
     << "		}\r\n\r\n"
     << "		.container {\r\n"
     << "			max-width: 600px;\r\n"
     << "			padding: 20px;\r\n"
     << "		}\r\n\r\n"
     << "		h1 {\r\n"
     << "			font-size: 48px;\r\n"
     << "			margin-bottom: 20px;\r\n"
     << "		}\r\n\r\n"
     << "		p {\r\n"
     << "			font-size: 18px;\r\n"
     << "		}\r\n\r\n"
     << "	</style>\r\n"
     << "</head>\r\n\r\n"
     << "<body>\r\n"
     << "	<div class=\"container\">\r\n"
     << "		<h1>" << code << " " << phrase << "</h1>\r\n"
     << "		<h2><a href=\"/\">Webserv</a></h2>\r\n"
     << "	</div>\r\n"
     << "</body>\r\n\r\n"
     << "</html>\r\n";
  return ss.str();
}

// デフォルトのエラーページHTMLをファイルから読み取り生成
// ファイルがなかったり読み取りエラーとなった場合はプログラムで生成
std::string HttpUtils::generateErrorPage(const std::string& file, int code,
                                         const std::string& phrase) {
  try {
    return HttpUtils::generatePage(file);
  } catch (ServerException& e) {
    return HttpUtils::generateErrorPage(code, phrase);
  }
}

std::string HttpUtils::generateDateValue(void) {
  // "Fri, 12 Apr 2024 01:12:16 GMT":29文字
  // %a(Fri) :3文字 曜日省略形
  // %d(12)  :2文字 Day
  // %b(Apr) :3文字 月名省略形
  // %Y(2024):4文字 Year
  // %H(01)  :2文字 Hour
  // %M(12)  :2文字 Min
  // %S(16)  :2文字 Sec
  const int buffer_size = 32;
  char buf[buffer_size];
  std::time_t raw_time;

  std::tm* timeinfo;
  std::time(&raw_time);
  timeinfo = std::gmtime(&raw_time);

  std::strftime(buf, buffer_size, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
  return std::string(buf);
}

// ファイルの拡張子に基づいてContent-Typeを決定
// TODO Content-Typeの仕様を調べる
std::string HttpUtils::generateContentType(const std::string& path) {
  const std::string extension(Utils::getExtension(path));
  if (extension == "csv") return "text/csv";
  if (extension == "html") return "text/html";
  if (extension == "css") return "text/css";
  if (extension == "js") return "text/javascript";
  if (extension == "json") return "application/json";
  if (extension == "pdf") return "application/pdf";
  if (extension == "png") return "image/apng";
  if (extension == "jpeg") return "image/jpeg";
  if (extension == "gif") return "image/gif";
  if (extension == "svg") return "image/svg+xml";
  if (extension == "zip") return "application/zip";
  if (extension == "mpeg") return "video/mpeg";
  return "text/plain";
}

// TODO KeepAliveをCloseするStatusCodeを調べる
bool HttpUtils::isMaintainConnection(int code) {
  std::vector<int> disconn_code;
  disconn_code.push_back(400);
  disconn_code.push_back(405);
  disconn_code.push_back(408);
  disconn_code.push_back(413);
  disconn_code.push_back(414);
  disconn_code.push_back(421);
  disconn_code.push_back(431);
  disconn_code.push_back(501);
  disconn_code.push_back(505);
  return (std::count(disconn_code.begin(), disconn_code.end(), code) == 0);
}
