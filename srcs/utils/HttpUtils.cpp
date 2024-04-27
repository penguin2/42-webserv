#include "HttpUtils.hpp"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <utility>

#include "FileUtils.hpp"
#include "ServerException.hpp"
#include "Utils.hpp"

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
std::string HttpUtils::generateErrorPage(const std::string* file, int code,
                                         const std::string& phrase) {
  std::stringstream ss;
  if (file == NULL ||
      FileUtils::readAllDataFromFile(file->c_str(), ss) == false)
    return HttpUtils::generateErrorPage(code, phrase);
  return ss.str();
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
std::string HttpUtils::convertPathToContentType(const std::string& path) {
  static const std::map<std::string, std::string> content_type_map =
      makeContentTypeMap();
  const std::string extension(Utils::getExtension(path));
  const std::map<std::string, std::string>::const_iterator it =
      content_type_map.find(extension);

  if (it != content_type_map.end()) return it->second;
  return "text/plain";
}

bool HttpUtils::isMaintainConnection(int code) {
  static const std::set<int> disconn_codes = HttpUtils::makeDisconnectCodeSet();
  return (disconn_codes.find(code) == disconn_codes.end());
}

bool HttpUtils::isRedirectStatusCode(int code) {
  static const std::set<int> redirect_codes = HttpUtils::makeRedirectCodeSet();
  return (redirect_codes.find(code) != redirect_codes.end());
}

// TODO Content-Typeの仕様を調べる
std::map<std::string, std::string> HttpUtils::makeContentTypeMap(void) {
  std::map<std::string, std::string> content_type_map;
  content_type_map.insert(std::make_pair("csv", "text/csv"));
  content_type_map.insert(std::make_pair("html", "text/html"));
  content_type_map.insert(std::make_pair("css", "text/css"));
  content_type_map.insert(std::make_pair("js", "text/javascript"));
  content_type_map.insert(std::make_pair("json", "application/json"));
  content_type_map.insert(std::make_pair("pdf", "application/pdf"));
  content_type_map.insert(std::make_pair("png", "image/apng"));
  content_type_map.insert(std::make_pair("jpeg", "image/jpeg"));
  content_type_map.insert(std::make_pair("gif", "image/gif"));
  content_type_map.insert(std::make_pair("svg", "image/svg+xml"));
  content_type_map.insert(std::make_pair("zip", "application/zip"));
  content_type_map.insert(std::make_pair("mpeg", "video/mpeg"));
  return content_type_map;
}

// TODO KeepAliveをCloseするStatusCodeを調べる
std::set<int> HttpUtils::makeDisconnectCodeSet(void) {
  std::set<int> disconnect_status_codes;
  disconnect_status_codes.insert(400);
  disconnect_status_codes.insert(405);
  disconnect_status_codes.insert(408);
  disconnect_status_codes.insert(413);
  disconnect_status_codes.insert(414);
  disconnect_status_codes.insert(421);
  disconnect_status_codes.insert(431);
  disconnect_status_codes.insert(501);
  disconnect_status_codes.insert(505);
  return disconnect_status_codes;
}

// リダイレクトは301,302,303,307,308のみ対応
std::set<int> HttpUtils::makeRedirectCodeSet(void) {
  std::set<int> redirect_status_codes;
  redirect_status_codes.insert(301);
  redirect_status_codes.insert(302);
  redirect_status_codes.insert(303);
  redirect_status_codes.insert(307);
  redirect_status_codes.insert(308);
  return redirect_status_codes;
}
