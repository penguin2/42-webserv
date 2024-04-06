#include <gtest/gtest.h>

#include "ServerException.hpp"
#include "Uri.hpp"

void testAbsoluteForm(std::string uri, const char* scheme,
                      const char* user_info, const char* host, int port,
                      const char* path, const char* query, const char* fragment,
                      bool expect) {
  Uri URI_instance;
  bool is_correct_request = true;

  try {
    URI_instance.parse(uri);
  } catch (ServerException& e) {
    is_correct_request = false;
  }
  ASSERT_EQ(is_correct_request, expect) << uri;
  if (is_correct_request == true) {
    EXPECT_STREQ(URI_instance.getScheme().c_str(), scheme);
    EXPECT_STREQ(URI_instance.getUserInfo().c_str(), user_info);
    EXPECT_STREQ(URI_instance.getHost().c_str(), host);
    EXPECT_EQ(URI_instance.getPort(), port);
    EXPECT_STREQ(URI_instance.getPath().c_str(), path);
    EXPECT_STREQ(URI_instance.getQuery().c_str(), query);
    EXPECT_STREQ(URI_instance.getFragment().c_str(), fragment);
  }
}

void testOriginForm(std::string uri, std::string host_header_value,
                    const char* scheme, const char* user_info, const char* host,
                    int port, const char* path, const char* query,
                    const char* fragment, bool expect) {
  Uri URI_instance;
  bool is_correct_request = true;

  try {
    URI_instance.parse(uri);
    URI_instance.overwriteAuthorityIfNotSet(host_header_value);
  } catch (ServerException& e) {
    is_correct_request = false;
  }
  ASSERT_EQ(is_correct_request, expect);
  if (is_correct_request == true) {
    EXPECT_STREQ(URI_instance.getScheme().c_str(), scheme);
    EXPECT_STREQ(URI_instance.getUserInfo().c_str(), user_info);
    EXPECT_STREQ(URI_instance.getHost().c_str(), host);
    EXPECT_EQ(URI_instance.getPort(), port);
    EXPECT_STREQ(URI_instance.getPath().c_str(), path);
    EXPECT_STREQ(URI_instance.getQuery().c_str(), query);
    EXPECT_STREQ(URI_instance.getFragment().c_str(), fragment);
  }
}

TEST(Uri, ABSOLUTE_FORM_SUCCESS) {
  // URI,scheme,userinfo,host,port,path,query,fragment,expect(bool)
  // expect(bool) URIの構文が正しい場合はtrue, 誤っている場合はfalse
  testAbsoluteForm("http://localhost/index.html?q=50#abc", "http", "",
                   "localhost", 80, "/index.html", "q=50", "abc", true);
  testAbsoluteForm("http://youser:pass@127.0.0.1/form.html", "http",
                   "youser:pass", "127.0.0.1", 80, "/form.html", "", "", true);
  testAbsoluteForm("http://@sample.com", "http", "", "sample.com", 80, "", "",
                   "", true);
  testAbsoluteForm("http://sample.com:4242/environ.py", "http", "",
                   "sample.com", 4242, "/environ.py", "", "", true);
  testAbsoluteForm("http://a:b@www.google.com:443/search.php?p=42#2", "http",
                   "a:b", "www.google.com", 443, "/search.php", "p=42", "2",
                   true);
  testAbsoluteForm("http://localhost", "http", "", "localhost", 80, "", "", "",
                   true);
  testAbsoluteForm("http://1:23/index.html", "http", "", "1", 23, "/index.html",
                   "", "", true);
}

TEST(Uri, ABSOLUTE_FORM_ERROR) {
  // expect(bool) == falseの場合、値の比較は行わないので適当な値で良い
  testAbsoluteForm("smtp://sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:/sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:///sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://sample.com:abc/index.html", "", "", "", -1, "", "",
                   "", false);
  testAbsoluteForm("http://:abc/index.html", "", "", "", -1, "", "", "", false);
}

TEST(Uri, ORIGIN_FORM_SUCCESS) {
  // URI,HostHeaderValue,scheme,userinfo,host,port,path,query,fragment,expect(bool)
  // expect(bool) URIの構文が正しい場合はtrue, 誤っている場合はfalse
  testOriginForm("/index.html?q=50#abc", "localhost", "http", "", "localhost",
                 80, "/index.html", "q=50", "abc", true);
  testOriginForm("/index.html?q=50#abc", "www.google.com", "http", "",
                 "www.google.com", 80, "/index.html", "q=50", "abc", true);
}

TEST(Uri, ORIGIN_FORM_ERROR) {
  // expect(bool) == falseの場合、値の比較は行わないので適当な値で良い
  testOriginForm("0.0.0.0", "smtp://sample.com", "", "", "", -1, "", "", "",
                 false);
}
