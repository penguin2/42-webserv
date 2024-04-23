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
  ASSERT_EQ(is_correct_request, expect) << uri << " " << host_header_value;
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
  testAbsoluteForm("http://0.0.0.0/./././././index.html", "http", "", "0.0.0.0",
                   80, "/index.html", "", "", true);
  testAbsoluteForm("http://localhost//////////////////", "http", "",
                   "localhost", 80, "/", "", "", true);
  testAbsoluteForm("http://localhost/?", "http", "", "localhost", 80, "/", "",
                   "", true);
  testAbsoluteForm("http://localhost/?#", "http", "", "localhost", 80, "/", "",
                   "", true);
  testAbsoluteForm("http://localhost/????#", "http", "", "localhost", 80, "/",
                   "???", "", true);
  testAbsoluteForm("http://localhost/#?q=50#", "http", "", "localhost", 80, "/",
                   "", "?q=50#", true);
  testAbsoluteForm("http://localhost/#?q=50#", "http", "", "localhost", 80, "/",
                   "", "?q=50#", true);
  testAbsoluteForm("http://localhost/space%20.html", "http", "", "localhost",
                   80, "/space .html", "", "", true);
  testAbsoluteForm("http://localhost/index.html?%E9%8D%B5=%E5%80%A4", "http",
                   "", "localhost", 80, "/index.html", "鍵=値", "", true);
  testAbsoluteForm("http://localhost?v=key", "http", "", "localhost", 80, "",
                   "v=key", "", true);
  testAbsoluteForm("http://localhost#f=N", "http", "", "localhost", 80, "", "",
                   "f=N", true);

  // CHATGPT
  testAbsoluteForm("http://example.com", "http", "", "example.com", 80, "", "",
                   "", true);
  testAbsoluteForm("http://example.com/", "http", "", "example.com", 80, "/",
                   "", "", true);
  testAbsoluteForm("http://www.example.com", "http", "", "www.example.com", 80,
                   "", "", "", true);
  testAbsoluteForm("http://www.example.com/path", "http", "", "www.example.com",
                   80, "/path", "", "", true);
  testAbsoluteForm("http://subdomain.example.com", "http", "",
                   "subdomain.example.com", 80, "", "", "", true);
  testAbsoluteForm("http://subdomain.example.com/path", "http", "",
                   "subdomain.example.com", 80, "/path", "", "", true);
  testAbsoluteForm("http://example.com:8080", "http", "", "example.com", 8080,
                   "", "", "", true);
  testAbsoluteForm("http://example.com:8080/path", "http", "", "example.com",
                   8080, "/path", "", "", true);
  testAbsoluteForm("http://user:password@example.com", "http", "user:password",
                   "example.com", 80, "", "", "", true);
  testAbsoluteForm("http://EXAMPLE.com", "http", "", "example.com", 80, "", "",
                   "", true);
  testAbsoluteForm("http://Example.com", "http", "", "example.com", 80, "", "",
                   "", true);
  testAbsoluteForm("http://eXaMpLe.cOm", "http", "", "example.com", 80, "", "",
                   "", true);
  testAbsoluteForm("http://example.com/path?Query=Value", "http", "",
                   "example.com", 80, "/path", "Query=Value", "", true);
  testAbsoluteForm("http://example.com/path?query=value&Param1=abc", "http", "",
                   "example.com", 80, "/path", "query=value&Param1=abc", "",
                   true);
  testAbsoluteForm("http://example.com/%20path", "http", "", "example.com", 80,
                   "/ path", "", "", true);
  testAbsoluteForm("http://example.com/%20path%20with%20spaces", "http", "",
                   "example.com", 80, "/ path with spaces", "", "", true);
  testAbsoluteForm("http://example.com/Path%20With%20Encoded%20Characters",
                   "http", "", "example.com", 80,
                   "/Path With Encoded Characters", "", "", true);
  testAbsoluteForm(
      "http://example.com/path?query=Value%20With%20Encoded%20Spaces", "http",
      "", "example.com", 80, "/path", "query=Value With Encoded Spaces", "",
      true);
  testAbsoluteForm(
      "http://example.com/"
      "path?query=value%20with%20encoded%20spaces&param1=ABC",
      "http", "", "example.com", 80, "/path",
      "query=value with encoded spaces&param1=ABC", "", true);
  testAbsoluteForm("http://user123:PassW0rD@example.com", "http",
                   "user123:PassW0rD", "example.com", 80, "", "", "", true);
  testAbsoluteForm("http://example.com/#Fragment", "http", "", "example.com",
                   80, "/", "", "Fragment", true);
  testAbsoluteForm("http://example.com/path#fragment", "http", "",
                   "example.com", 80, "/path", "", "fragment", true);
  testAbsoluteForm("http://example.com/path#Fragment?query=value", "http", "",
                   "example.com", 80, "/path", "", "Fragment?query=value",
                   true);
  testAbsoluteForm("http://EXAMPLE.com:8080", "http", "", "example.com", 8080,
                   "", "", "", true);
  testAbsoluteForm("http://example.COM:8080", "http", "", "example.com", 8080,
                   "", "", "", true);
  testAbsoluteForm("http://Example.com:8080", "http", "", "example.com", 8080,
                   "", "", "", true);
  testAbsoluteForm("http://exaMPle.com:8080", "http", "", "example.com", 8080,
                   "", "", "", true);
  testAbsoluteForm("http://example.com:8443/path?query=value", "http", "",
                   "example.com", 8443, "/path", "query=value", "", true);
  testAbsoluteForm("http://example.com/path?query=value#Fragment", "http", "",
                   "example.com", 80, "/path", "query=value", "Fragment", true);
  testAbsoluteForm("http://example.com/path?query=value#Fragment123", "http",
                   "", "example.com", 80, "/path", "query=value", "Fragment123",
                   true);
  testAbsoluteForm("http://example.com?query#fragment", "http", "",
                   "example.com", 80, "", "query", "fragment", true);
  testAbsoluteForm("http://example.com?query#", "http", "", "example.com", 80,
                   "", "query", "", true);
  testAbsoluteForm("http://example.com?query&value", "http", "", "example.com",
                   80, "", "query&value", "", true);
  testAbsoluteForm("http://example.com?query#fragment#", "http", "",
                   "example.com", 80, "", "query", "fragment#", true);
  testAbsoluteForm("http://example.com?query?", "http", "", "example.com", 80,
                   "", "query?", "", true);
  testAbsoluteForm("http://example.com?query=?", "http", "", "example.com", 80,
                   "", "query=?", "", true);
  testAbsoluteForm("http://example.com?query=&", "http", "", "example.com", 80,
                   "", "query=&", "", true);
}

TEST(Uri, ABSOLUTE_FORM_ERROR) {
  // expect(bool) == falseの場合、値の比較は行わないので適当な値で良い
  testAbsoluteForm("://localhost", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("smtp://sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:://google.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:/sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:///sample.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://sample.com:abc/index.html", "", "", "", -1, "", "",
                   "", false);
  testAbsoluteForm("http://:abc/index.html", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://#/localhost/index.html", "", "", "", -1, "", "", "",
                   false);
  testAbsoluteForm("%48ttp://localhost/", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://localhost:%38%30/", "", "", "", -1, "", "", "",
                   false);
  testAbsoluteForm("http://@/index.html", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://:@:/index.html", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://:example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://\"localhost\"/", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://:@255.255.255.255:255/../bin/ls", "", "", "", -1, "",
                   "", "", false);

  // CHATGPT
  testAbsoluteForm("http:/example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http//example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:example.com/path", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:://example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://:example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http:example.com/path?query=value", "", "", "", -1, "", "",
                   "", false);
  testAbsoluteForm("http:/example.com/path#fragment", "", "", "", -1, "", "",
                   "", false);
  testAbsoluteForm("http:/example.com:port/path", "", "", "", -1, "", "", "",
                   false);
  testAbsoluteForm("http:/@example.com", "", "", "", -1, "", "", "", false);
  testAbsoluteForm("http://example.com?query=%", "", "", "", -1, "", "", "",
                   false);
  testAbsoluteForm("http://example.com?query=%1", "", "", "", -1, "", "", "",
                   false);
  testAbsoluteForm("http://example.com?query=%1G", "", "", "", -1, "", "", "",
                   false);
  testAbsoluteForm("http://example.com?query=%1G2", "", "", "", -1, "", "", "",
                   false);
}

TEST(Uri, ORIGIN_FORM_SUCCESS) {
  // URI,HostHeaderValue,scheme,userinfo,host,port,path,query,fragment,expect(bool)
  // expect(bool) URIの構文が正しい場合はtrue, 誤っている場合はfalse
  testOriginForm("/index.html?q=50#abc", "localhost", "http", "", "localhost",
                 80, "/index.html", "q=50", "abc", true);
  testOriginForm("/index.html?q=50#abc", "www.google.com", "http", "",
                 "www.google.com", 80, "/index.html", "q=50", "abc", true);

  // CHATGPT
  testOriginForm("/index.html", "www.example.com", "http", "",
                 "www.example.com", 80, "/index.html", "", "", true);
  testOriginForm("/path/to/resource", "api.example.com", "http", "",
                 "api.example.com", 80, "/path/to/resource", "", "", true);
  testOriginForm("/page.html", "subdomain.example.com", "http", "",
                 "subdomain.example.com", 80, "/page.html", "", "", true);
  testOriginForm("/dir/document.pdf", "example.com", "http", "", "example.com",
                 80, "/dir/document.pdf", "", "", true);
  testOriginForm("/file.txt", "www.subdomain.example.com", "http", "",
                 "www.subdomain.example.com", 80, "/file.txt", "", "", true);
  testOriginForm("/folder/image.jpg", "www.example.org", "http", "",
                 "www.example.org", 80, "/folder/image.jpg", "", "", true);
  testOriginForm("/page.html", "sub.sub.sub.example.com", "http", "",
                 "sub.sub.sub.example.com", 80, "/page.html", "", "", true);
  testOriginForm("/gallery", "gallery.example~com", "http", "",
                 "gallery.example~com", 80, "/gallery", "", "", true);
  testOriginForm("/page.html", "subdomain.example.com,", "http", "",
                 "subdomain.example.com,", 80, "/page.html", "", "", true);
}

TEST(Uri, ORIGIN_FORM_ERROR) {
  // expect(bool) == falseの場合、値の比較は行わないので適当な値で良い
  testOriginForm("0.0.0.0", "smtp://sample.com", "", "", "", -1, "", "", "",
                 false);
  // CHATGPT
  testOriginForm("index.html", "www.example.com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/dir/document.pdf", "example.com?query", "", "", "", -1, "",
                 "", "", false);
  testOriginForm("/file.txt", "www.subdomain.example.com#fragment", "", "", "",
                 -1, "", "", "", false);
  testOriginForm("/page.html", "sub.sub.sub.example.com@", "", "", "", -1, "",
                 "", "", false);
  testOriginForm("/search", "search%example.com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/privacy", "privacy.example:com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/profile", "profile.example<com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/settings", "settings.example>com", "", "", "", -1, "", "",
                 "", false);
  testOriginForm("/help", "help.example[com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/faq", "faq.example]com", "", "", "", -1, "", "", "", false);
  testOriginForm("/support", "support.example{com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/news", "news.example}com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/events", "events.example|com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/blog", "blog.example^com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/forum", "forum.example`com", "", "", "", -1, "", "", "",
                 false);
  testOriginForm("/api", "api.example\\com", "", "", "", -1, "", "", "", false);
  testOriginForm("/documentation", "docs.example|com", "", "", "", -1, "", "",
                 "", false);
}
