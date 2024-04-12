#ifndef WEBSERV_RESPONSEDATA_H_
#define WEBSERV_RESPONSEDATA_H_

#include <map>
#include <string>

class ResponseData {
 public:
  ResponseData(void);
  ~ResponseData(void);

  int getStatusCode(void) const;
  const std::string &getPhrase(void) const;
  const std::map<std::string, std::string> &getHeaders(void) const;
  const std::string &getBody(void) const;

  void setStatusCode(int code);
  void setPhrase(const std::string &phrase);
  void appendBody(const std::string &str);
  // keyとvalueを渡してセットする一般的なインターフェース
  void insertHeader(const std::string &key, const std::string &value);

  // TODO CGI用のHeader1行を丸々解析してinsertする関数
  // void insertHeader(const std::string &line);

 private:
  int status_code_;
  std::string phrase_;
  std::map<std::string, std::string> headers_;
  std::string body_;

  ResponseData(const ResponseData &);
  void operator=(const ResponseData &);
};

#endif
