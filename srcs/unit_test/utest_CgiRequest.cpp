#include <gtest/gtest.h>

#include "CgiRequest.hpp"
#include "Request.hpp"
#include "SocketAddress.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"

using namespace std;

class CreateCgiRequestTest {
 public:
  // LocationConfig->Rootデフォルト引数で"/lib"を設定しています
  CreateCgiRequestTest(string request_raw_str, string root = "/lib",
                       string server_name = "") {
    SocketAddress peer_addr("1.2.3.4", "NO USE PARAM");
    Request request;
    request.parse(request_raw_str);
    server_conf_ = new ServerConfig;
    server_conf_->setListenAddress(peer_addr.getIpAddr());
    server_conf_->setListenPort("4242");
    server_conf_->addLocationConfig("/");
    server_conf_->getLocationConfig("/").setRoot(root);
    server_conf_->getLocationConfig("/").addCgiExt(".py");
    if (!server_name.empty()) server_conf_->setServerName(server_name);
    request.setServerConfig(*server_conf_);
    cgi_request_ = CgiRequest::createCgiRequest(request, peer_addr);
  }

  ~CreateCgiRequestTest(void) {
    delete cgi_request_;
    delete server_conf_;
  }

  string getFilename(void) { return cgi_request_->getFilename(); }
  string getDirectory(void) { return cgi_request_->getDirectory(); }
  string getMessage(void) { return cgi_request_->getMessage(); }

  // envVars[key]が存在するならvalueとexpectを比較
  // 存在しないなら空文字列とexpectを比較
  // (メタ変数がsetされていないことをテストするにはexpectに""を指定)
  void expectEnvVar(string key, string expect) {
    map<string, string> env_vars = cgi_request_->getEnvVars();

    if (env_vars.find(key) != env_vars.end())
      EXPECT_STREQ(env_vars[key].c_str(), expect.c_str()) << key;
    else
      EXPECT_STREQ("", expect.c_str()) << key;
  }

 private:
  ServerConfig* server_conf_;
  CgiRequest* cgi_request_;
};

// リクエスト毎に変わらないメタ変数をテスト
void testCreateCgiRequestCommon(string request_raw_str) {
  CreateCgiRequestTest cgi_request(request_raw_str);
  cgi_request.expectEnvVar("GATEWAY_INTERFACE", "CGI/1.1");
  cgi_request.expectEnvVar("REMOTE_ADDR", "1.2.3.4");
  cgi_request.expectEnvVar("REMOTE_HOST", "1.2.3.4");
  cgi_request.expectEnvVar("SERVER_PORT", "4242");
  cgi_request.expectEnvVar("SERVER_SOFTWARE", "Webserv");
  cgi_request.expectEnvVar("SERVER_PROTOCOL", "HTTP/1.1");
  cgi_request.expectEnvVar("SERVER_NAME", "");

  CreateCgiRequestTest cgi_request_set_server_name(request_raw_str, "",
                                                   "localhost");
  cgi_request_set_server_name.expectEnvVar("SERVER_NAME", "localhost");
}

void testCreateCgiRequestFileData(string request_raw_str, string directory,
                                  string filename, string path_info,
                                  string path_translated, string script_name) {
  CreateCgiRequestTest cgi_request(request_raw_str);
  EXPECT_STREQ(cgi_request.getDirectory().c_str(), directory.c_str());
  EXPECT_STREQ(cgi_request.getFilename().c_str(), filename.c_str());
  cgi_request.expectEnvVar("PATH_INFO", path_info.c_str());
  cgi_request.expectEnvVar("PATH_TRANSLATED", path_translated.c_str());
  cgi_request.expectEnvVar("SCRIPT_NAME", script_name.c_str());
}

void testCreateCgiRequestFileDataSpRootCase(string sp_root_str,
                                            string request_raw_str,
                                            string directory, string filename,
                                            string path_info,
                                            string path_translated,
                                            string script_name) {
  CreateCgiRequestTest cgi_request_set_root(request_raw_str, sp_root_str, "");
  EXPECT_STREQ(cgi_request_set_root.getDirectory().c_str(), directory.c_str());
  EXPECT_STREQ(cgi_request_set_root.getFilename().c_str(), filename.c_str());
  cgi_request_set_root.expectEnvVar("PATH_INFO", path_info.c_str());
  cgi_request_set_root.expectEnvVar("PATH_TRANSLATED", path_translated.c_str());
  cgi_request_set_root.expectEnvVar("SCRIPT_NAME", script_name.c_str());
}

void testCreateCgiRequestBody(std::string request_raw_str, std::string message,
                              std::string content_length,
                              std::string contnet_type) {
  CreateCgiRequestTest cgi_request(request_raw_str);
  EXPECT_STREQ(cgi_request.getMessage().c_str(), message.c_str());
  cgi_request.expectEnvVar("CONTENT_LENGTH", content_length.c_str());
  cgi_request.expectEnvVar("CONTENT_TYPE", contnet_type.c_str());
}

void testCreateCgiRequestUriData(string request_raw_str, string method,
                                 string query) {
  CreateCgiRequestTest cgi_request(request_raw_str);
  cgi_request.expectEnvVar("REQUEST_METHOD", method.c_str());
  cgi_request.expectEnvVar("QUERY_STRING", query.c_str());
}

// HTTPリクエストから渡されたheaderをテスト
void testCreateCgiRequestHTTPHeaders(string request_raw_str,
                                     map<string, string> headers) {
  CreateCgiRequestTest cgi_request(request_raw_str);

  for (map<string, string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    cgi_request.expectEnvVar(it->first, it->second);
  }
}

TEST(CgiRequest, COMMON) {
  // HTTPリクエスト
  testCreateCgiRequestCommon("GET / HTTP/1.1\r\nHost: 1\r\n\r\n");
  testCreateCgiRequestCommon("GET / HTTP/1.1\r\nhost: WWW.GOOGLE.COM\r\n\r\n");
}

TEST(CgiRequest, FILE_DATA) {
  // HTTPリクエスト, DIRECTORY, FILE, PATH_INFO, PATH_TRANSLATED, SCRIPT_NAME
  // CGIであることが確定したシナリオなのでPathに.py拡張子を含めてテストしてください
  // (cgi_extがなければそもそもCgiRequest::createCgiRequest()は呼ばれません)
  // rootディレクティブの値を"/lib"に設定
  testCreateCgiRequestFileData(
      "GET /cgi-bin/test.py/abc HTTP/1.1\r\nHost: 1\r\n\r\n", "/lib/cgi-bin/",
      "test.py", "/abc", "/lib/abc", "/cgi-bin/test.py");
  testCreateCgiRequestFileData("GET /a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "/lib/",
                               "a.py", "", "", "/a.py");
  testCreateCgiRequestFileData("GET //////a.py HTTP/1.1\r\nHost: 1\r\n\r\n",
                               "/lib/", "a.py", "", "", "/a.py");
  testCreateCgiRequestFileData("GET /././a.py HTTP/1.1\r\nHost: 1\r\n\r\n",
                               "/lib/", "a.py", "", "", "/a.py");
}

TEST(CgiRequest, FILE_DATA_SP_ROOT) {
  // ROOT, REQUEST, DIRECTORY, FILE, PATH_INFO, PATH_TRANSLATED, SCRIPT_NAME
  const string ROOT_STR1 = "/";
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR1, "GET /cgi-bin/test.py/abc HTTP/1.1\r\nHost: 1\r\n\r\n",
      "/cgi-bin/", "test.py", "/abc", "/abc", "/cgi-bin/test.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR1, "GET /a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "/", "a.py", "", "",
      "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR1, "GET //////a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "/", "a.py", "",
      "", "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR1, "GET /././a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "/", "a.py", "",
      "", "/a.py");

  const string ROOT_STR2 = "../";
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR2, "GET /cgi-bin/test.py/abc HTTP/1.1\r\nHost: 1\r\n\r\n",
      "../cgi-bin/", "test.py", "/abc", "../abc", "/cgi-bin/test.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR2, "GET /a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "../", "a.py", "", "",
      "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR2, "GET //////a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "../", "a.py",
      "", "", "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR2, "GET /././a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "../", "a.py", "",
      "", "/a.py");

  const string ROOT_STR3 = "./";
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR3, "GET /a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "./", "a.py", "", "",
      "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR3, "GET /././a.py/abc/ HTTP/1.1\r\nHost: 1\r\n\r\n", "./", "a.py",
      "/abc/", "./abc/", "/a.py");

  const string ROOT_STR4 = "current_path/";
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR4, "GET /a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "current_path/",
      "a.py", "", "", "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR4, "GET /123/a.py/abc/ HTTP/1.1\r\nHost: 1\r\n\r\n",
      "current_path/123/", "a.py", "/abc/", "current_path/abc/", "/123/a.py");

  const string ROOT_STR5 = "path";
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR5, "GET /a.py HTTP/1.1\r\nHost: 1\r\n\r\n", "path/", "a.py", "",
      "", "/a.py");
  testCreateCgiRequestFileDataSpRootCase(
      ROOT_STR5, "GET /123/a.py/abc/ HTTP/1.1\r\nHost: 1\r\n\r\n", "path/123/",
      "a.py", "/abc/", "path/abc/", "/123/a.py");
}
TEST(CgiRequest, BODY) {
  // HTTPリクエスト, BODY, CONTENT_LENGTH, CONTENT_TYPE
  testCreateCgiRequestBody(
      "GET / HTTP/1.1\r\nHost: 1\r\nContent-Length: 10\r\n\r\n1234567890",
      "1234567890", "10", "");
  testCreateCgiRequestBody(
      "GET / HTTP/1.1\r\nHost: 1\r\nContent-Type: text/html\r\n\r\n", "", "",
      "text/html");
  testCreateCgiRequestBody(
      "GET / HTTP/1.1\r\nHost: 1\r\nContent-Type: "
      "text/html\r\ncONTENT-lENGTH:1\r\n\r\na",
      "a", "1", "text/html");
  testCreateCgiRequestBody("GET / HTTP/1.1\r\nHost: 1\r\n\r\n", "", "", "");
  testCreateCgiRequestBody(
      "GET / HTTP/1.1\r\nHost: 1\r\nContent-Length:0\r\n\r\n", "", "", "");
  // Chunk Pattern
  testCreateCgiRequestBody(
      "GET / HTTP/1.1\r\nHost: 1\r\nTransfer-Encoding: "
      "chunked\r\n\r\n1\r\na\r\n0\r\n",
      "a", "1", "");
  testCreateCgiRequestBody(
      "GET / HTTP/1.1\r\nHost: 1\r\nTransfer-Encoding: "
      "chunked\r\n\r\n2\r\n42\r\n6\r\n-Tokyo\r\n0\r\n",
      "42-Tokyo", "8", "");
}

TEST(CgiRequest, URI_DATA) {
  // HTTPリクエスト, METHOD, QUERY_STRING
  testCreateCgiRequestUriData("GET / HTTP/1.1\r\nHost:1\r\n\r\n", "GET", "");
  testCreateCgiRequestUriData("POST /?q=50 HTTP/1.1\r\nHost:1\r\n\r\n", "POST",
                              "q=50");
  testCreateCgiRequestUriData("DELETE /? HTTP/1.1\r\nHost:1\r\n\r\n", "DELETE",
                              "");
  testCreateCgiRequestUriData(
      "GET /index.html?c=%20 HTTP/1.1\r\nHost:1\r\n\r\n", "GET", "c=%20");
  testCreateCgiRequestUriData("GET /?q=50#F HTTP/1.1\r\nHost:1\r\n\r\n", "GET",
                              "q=50");
}

TEST(CgiRequest, HTTP_HEADERS) {
  // HTTPリクエスト, map<string(CGI key), string(expect_value)> expect_headers
  testCreateCgiRequestHTTPHeaders(
      "GET / HTTP/1.1\r\nHost: 1\r\nMYVAR: 1\r\nmy-var_2:2\r\n\r\n",
      {{"HTTP_MYVAR", "1"}, {"HTTP_MY_VAR_2", "2"}});
  testCreateCgiRequestHTTPHeaders(
      "GET / HTTP/1.1\r\nHost: 1\r\nContent-Length:10\r\n\r\n1234567890",
      {{"HTTP_HOST", "1"}, {"HTTP_CONTENT_LENGTH", "10"}});
  testCreateCgiRequestHTTPHeaders(
      "GET / HTTP/1.1\r\nHost: 1\r\nTransfer-Encoding: "
      "chunked\r\n\r\n1\r\nA\r\n0\r\n",
      {{"HTTP_TRANSFER_ENCODING", "chunked"}, {"HTTP_CONTENT_LENGTH", "1"}});
}
