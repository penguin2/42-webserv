#include "HttpUtils.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

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

std::string HttpUtils::generateDateAsFormat(std::time_t t,
                                            const std::string& fmt) {
  const int buffer_size = 256;
  char buf[buffer_size];

  std::strftime(buf, buffer_size, fmt.c_str(), std::gmtime(&t));
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

bool HttpUtils::generateAutoindexPage(const std::string& path_component,
                                      const std::string& absolute_dir_path,
                                      std::stringstream& ss) {
  std::vector<struct dirent> dir_data =
      FileUtils::readDirData(absolute_dir_path);
  if (dir_data.size() == 0) return false;
  std::sort(dir_data.begin(), dir_data.end(), AutoindexUtils::compareDirent);

  ss << "<html>\r\n"
     << "<head>\r\n"
     << "<title>Index of " << path_component << "</title>\r\n"
     << "</head>\r\n"
     << "<body>\r\n"
     << "<h1>Index of " << path_component << "</h1>\r\n"
     << "<hr>\r\n"
     << "<pre>\r\n"
     << "<a href=\"../\">../</a>\r\n";
  for (std::vector<struct dirent>::const_iterator it = dir_data.begin();
       it != dir_data.end(); ++it) {
    if (AutoindexUtils::generateFileRecord(*it, absolute_dir_path, ss) == false)
      return false;
  }
  ss << "<pre>\r\n"
     << "<hr>\r\n"
     << "</body>\r\n"
     << "</html>\r\n";
  return true;
}

bool HttpUtils::AutoindexUtils::generateFileRecord(const struct dirent& entry,
                                                   const std::string& dir,
                                                   std::stringstream& ss) {
  const std::string absolute_path(dir + "/" + entry.d_name);

  if (entry.d_name == std::string(".") || entry.d_name == std::string(".."))
    return true;
  bool is_dir_file_type = (entry.d_type == DT_DIR);
  std::string file_name(entry.d_name);
  if (is_dir_file_type) file_name.push_back('/');
  AutoindexUtils::generateFileLink(file_name, ss);
  if (AutoindexUtils::generateFileDetail(absolute_path, is_dir_file_type, ss) ==
      false)
    return false;
  return true;
}

void HttpUtils::AutoindexUtils::generateFileLink(const std::string& file_name,
                                                 std::stringstream& ss) {
  const int n = 50;

  ss << "<a href=\"" << file_name << "\">";
  if (file_name.size() <= n)
    ss << file_name;
  else
    ss << file_name.substr(0, n - 3) << "..>";
  ss << "</a> ";
  if (file_name.size() <= n) {
    for (size_t i = 0; i < (n - file_name.size()); i++) ss << ' ';
  }
}

bool HttpUtils::AutoindexUtils::generateFileDetail(const std::string& file_path,
                                                   bool is_dir,
                                                   std::stringstream& ss) {
  const int n = 20;
  struct stat st;

  if (stat(file_path.c_str(), &st) < 0) return false;
  ss << generateDateAsFormat(st.st_mtime, "%d-%b-%Y %H:%M");
  if (is_dir) {
    for (size_t i = 1; i < n; i++) ss << ' ';
    ss << '-';
  } else {
    off_t file_size = FileUtils::getFileSize(file_path);
    if (file_size < 0) return false;
    std::stringstream ss_file_size;
    ss_file_size << file_size;
    for (size_t i = ss_file_size.str().size(); i < n; i++) ss << ' ';
    ss << ss_file_size.str();
  }
  ss << " \r\n";
  return true;
}

bool HttpUtils::AutoindexUtils::compareDirent(struct dirent& entry1,
                                              struct dirent& entry2) {
  if (entry1.d_type == entry2.d_type)
    return (std::string(entry1.d_name) < std::string(entry2.d_name));
  if (entry1.d_type == DT_DIR) return true;
  return false;
}
