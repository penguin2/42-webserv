#include "HttpUtils.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

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
std::string HttpUtils::generateErrorPage(size_t status_code,
                                         const std::string& phrase) {
  std::stringstream ss;

  ss << "<!DOCTYPE html>\r\n"
     << "<html lang=\"en\">\r\n\r\n"
     << "<head>\r\n"
     << "	<meta charset=\"UTF-8\">\r\n"
     << "	<meta name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1.0\">\r\n"
     << "	<title>" << status_code << " " << phrase << "</title>\r\n"
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
     << "		<h1>" << status_code << " " << phrase << "</h1>\r\n"
     << "		<h2><a href=\"/\">Webserv</a></h2>\r\n"
     << "	</div>\r\n"
     << "</body>\r\n\r\n"
     << "</html>\r\n";
  return ss.str();
}

// デフォルトのエラーページHTMLをファイルから読み取り生成
// ファイルがなかったり読み取りエラーとなった場合はプログラムで生成
std::string HttpUtils::generateErrorPage(const std::string* file,
                                         size_t status_code,
                                         const std::string& phrase) {
  std::stringstream ss;
  if (file == NULL || !FileUtils::isExistFile(*file) ||
      !FileUtils::hasFilePermission(*file, R_OK) ||
      FileUtils::readAllDataFromFile(file->c_str(), ss) == false)
    return HttpUtils::generateErrorPage(status_code, phrase);
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
std::string HttpUtils::convertPathToContentType(const std::string& file_path) {
  static const std::map<std::string, std::string> content_type_map =
      makeContentTypeMap();
  const std::string extension(Utils::getExtension(file_path));
  const std::map<std::string, std::string>::const_iterator it =
      content_type_map.find(extension);

  if (it != content_type_map.end()) return it->second;
  return "text/plain";
}

bool HttpUtils::isKeepConnection(size_t code) {
  static const std::set<size_t> keep_conn_codes =
      HttpUtils::makeKeepConnectionCodeSet();
  return (keep_conn_codes.find(code) != keep_conn_codes.end());
}

bool HttpUtils::isRedirectStatusCode(size_t code) {
  static const std::set<size_t> redirect_codes =
      HttpUtils::makeRedirectCodeSet();
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

// リダイレクトと静的ファイルの正常系ステータスコードのみKeepAlive
std::set<size_t> HttpUtils::makeKeepConnectionCodeSet(void) {
  std::set<size_t> keep_connection_codes = makeRedirectCodeSet();
  keep_connection_codes.insert(200);
  keep_connection_codes.insert(201);
  keep_connection_codes.insert(204);
  return keep_connection_codes;
}

// リダイレクトは301,302,303,307,308のみ対応
std::set<size_t> HttpUtils::makeRedirectCodeSet(void) {
  std::set<size_t> redirect_status_codes;
  redirect_status_codes.insert(301);
  redirect_status_codes.insert(302);
  redirect_status_codes.insert(303);
  redirect_status_codes.insert(307);
  redirect_status_codes.insert(308);
  return redirect_status_codes;
}

bool HttpUtils::generateAutoindexPage(const std::string& path_component,
                                      const std::string& dir_path,
                                      std::stringstream& ss) {
  std::vector<FileUtils::Entry> dir_data =
      FileUtils::Entry::readDirData(dir_path);
  if (dir_data.size() == 0) return false;
  std::sort(dir_data.begin(), dir_data.end());

  ss << "<html>\r\n"
     << "<head>\r\n"
     << "<title>Index of " << path_component << "</title>\r\n"
     << "</head>\r\n"
     << "<body>\r\n"
     << "<h1>Index of " << path_component << "</h1>\r\n"
     << "<hr>\r\n"
     << "<pre>\r\n"
     << "<a href=\"../\">../</a>\r\n";
  for (std::vector<FileUtils::Entry>::const_iterator it = dir_data.begin();
       it != dir_data.end(); ++it) {
    if (it->startWithDot() == true ||
        it->getFileType() == FileUtils::Entry::UNKNOWN)
      continue;
    if (AutoindexUtils::generateFileRecord(*it, dir_path, ss) == false)
      return false;
    ss << "\r\n";
  }
  ss << "<pre>\r\n"
     << "<hr>\r\n"
     << "</body>\r\n"
     << "</html>\r\n";
  return true;
}

bool HttpUtils::AutoindexUtils::generateFileRecord(
    const FileUtils::Entry& entry, const std::string& dir,
    std::stringstream& ss) {
  const std::string file_path(dir + "/" + entry.getFileName());

  std::string file_name(entry.getFileName());
  const bool is_dir_type = (entry.getFileType() == FileUtils::Entry::DIRECTORY);
  if (is_dir_type) file_name.push_back('/');
  AutoindexUtils::generateFileLink(file_name, ss);
  if (AutoindexUtils::generateFileDetail(file_path, is_dir_type, ss) == false)
    return false;
  return true;
}

void HttpUtils::AutoindexUtils::generateFileLink(const std::string& file_name,
                                                 std::stringstream& ss) {
  static const int MAX_FILE_NAME_SIZE = 50;

  ss << "<a href=\"" << file_name << "\">";
  if (file_name.size() <= MAX_FILE_NAME_SIZE)
    ss << file_name;
  else
    ss << file_name.substr(0, MAX_FILE_NAME_SIZE - 3) << "..>";
  ss << "</a> ";
  if (file_name.size() <= MAX_FILE_NAME_SIZE) {
    ss << std::string(MAX_FILE_NAME_SIZE - file_name.size(), ' ');
  }
}

bool HttpUtils::AutoindexUtils::generateFileDetail(const std::string& file_path,
                                                   bool is_dir,
                                                   std::stringstream& ss) {
  static const int MAX_FILE_DETAIL_SIZE = 20;
  struct stat st;

  if (stat(file_path.c_str(), &st) < 0) return false;
  ss << generateDateAsFormat(st.st_mtime, "%d-%b-%Y %H:%M");
  if (is_dir) {
    ss << std::string(MAX_FILE_DETAIL_SIZE - 1, ' ') << '-';
  } else {
    off_t file_size = FileUtils::getFileSize(file_path);
    if (file_size < 0) return false;
    std::stringstream ss_file_size;
    ss_file_size << file_size;
    ss << std::string(MAX_FILE_DETAIL_SIZE - ss_file_size.str().size(), ' ');
    ss << ss_file_size.str();
  }
  return true;
}

bool HttpUtils::isStatusCode(size_t status_code) {
  return (100 <= status_code && status_code < 1000);
}

int HttpUtils::isHeaderKeyChar(int c) {
  std::string str;
  str.push_back(c);

  return (std::isalnum(c) || Utils::isContain(str, "!#$%&'*+-.^_`"));
}

int HttpUtils::isCookieValueChar(int c) {
  if (c == 0x21) return true;
  if (0x23 <= c && c <= 0x2B) return true;
  if (0x2D <= c && c <= 0x3A) return true;
  if (0x3C <= c && c <= 0x5B) return true;
  if (0x5D <= c && c <= 0x7E) return true;
  return false;
}

bool HttpUtils::isFullDateRFC1123(const std::string& date_str) {
  // RFC1123-date = wkday "," SP date1 SP time SP "GMT"
  // wkday = ("Mon" | "Tue" | "Wed" | "Thu" | "Fri" | "Sat" | "Sun")
  // date1 = 2DIGIT(day) SP month SP 4DIGIT(year)
  // month = ("Jan" | "Feb" | "Mar" | "Apr" | "May" | "Jun"
  //          "Jul" | "Aug" | "Sep" | "Oct" | "Nov" | "Dec")
  // time = 2DIGIT ":" 2DIGIT ":" 2DIGIT (00:00:00 - 23:59:59)
  static const size_t total_date_size = (3 + 1 + 1 + 11 + 1 + 8 + 1 + 3);
  if (date_str.size() != total_date_size) return false;
  if (!IsFullDateUtils::isWkDay(date_str.substr(0, 3))) return false;
  if (date_str.substr(3, 2) != ", ") return false;
  if (!IsFullDateUtils::isDate1(date_str.substr(5, 11))) return false;
  if (date_str[16] != ' ') return false;
  if (!IsFullDateUtils::isTime(date_str.substr(17, 8))) return false;
  if (date_str.substr(25) != " GMT") return false;
  return true;
}

bool HttpUtils::IsFullDateUtils::isWkDay(const std::string& wkday_str) {
  static const std::vector<std::string> wkdays = makeWkDay();

  return (std::find(wkdays.begin(), wkdays.end(), wkday_str) != wkdays.end());
}

std::vector<std::string> HttpUtils::IsFullDateUtils::makeWkDay(void) {
  std::vector<std::string> wkday_vec;

  wkday_vec.push_back("Mon");
  wkday_vec.push_back("Tue");
  wkday_vec.push_back("Wed");
  wkday_vec.push_back("Thu");
  wkday_vec.push_back("Fri");
  wkday_vec.push_back("Sat");
  wkday_vec.push_back("Sun");
  return wkday_vec;
}

bool HttpUtils::IsFullDateUtils::isDate1(const std::string& date1_str) {
  if (!Utils::isContainsOnly(date1_str.substr(0, 2), isdigit)) return false;
  if (date1_str[2] != ' ') return false;
  if (!isMonth(date1_str.substr(3, 3))) return false;
  if (date1_str[6] != ' ') return false;
  if (!Utils::isContainsOnly(date1_str.substr(7), isdigit)) return false;
  return true;
}

bool HttpUtils::IsFullDateUtils::isMonth(const std::string& month_str) {
  static const std::vector<std::string> months = makeMonth();

  return (std::find(months.begin(), months.end(), month_str) != months.end());
}

std::vector<std::string> HttpUtils::IsFullDateUtils::makeMonth(void) {
  std::vector<std::string> months;

  months.push_back("Jan");
  months.push_back("Feb");
  months.push_back("Mar");
  months.push_back("Apr");
  months.push_back("May");
  months.push_back("Jun");
  months.push_back("Jul");
  months.push_back("Aug");
  months.push_back("Sep");
  months.push_back("Oct");
  months.push_back("Nov");
  months.push_back("Dec");
  return months;
}

bool HttpUtils::IsFullDateUtils::isTime(const std::string& time_str) {
  if (!Utils::isContainsOnly(time_str.substr(0, 2), isdigit)) return false;
  if (time_str[2] != ':') return false;
  if (!Utils::isContainsOnly(time_str.substr(3, 2), isdigit)) return false;
  if (time_str[5] != ':') return false;
  if (!Utils::isContainsOnly(time_str.substr(6), isdigit)) return false;
  return true;
}
