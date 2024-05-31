#include <gtest/gtest.h>

#include <Cookie.hpp>
#include <CookieManager.hpp>

using namespace std;

void testParseCookie(string set_cookie_header_value, bool expect) {
  CookieManager manager;

  ASSERT_EQ(manager.parseCookie(set_cookie_header_value), expect)
      << set_cookie_header_value;
}

void testCookieManager(vector<string> set_cookie_header_values, string key,
                       string expect_cookie_value) {
  CookieManager manager;

  for (vector<string>::const_iterator it = set_cookie_header_values.begin();
       it != set_cookie_header_values.end(); ++it) {
    manager.parseCookie(*it);
  }

  map<string, Cookie> cookies = manager.getCookies();
  if (cookies.find(key) != cookies.end()) {
    Cookie cookie = cookies[key];
    stringstream ss;
    ss << cookie;
    string cookie_value = ss.str();
    ASSERT_STREQ(cookie_value.c_str(), expect_cookie_value.c_str());
  } else {
    ASSERT_STREQ("", expect_cookie_value.c_str());
  }
}

TEST(CookieManager, KEY_VALUE) {
  testParseCookie("KEY=VALUE", true);
  testParseCookie("key=value", true);
  testParseCookie("Key=Value", true);
  testParseCookie("Key=\"Value\"", true);
  testParseCookie("Key='Value'", true);
  testParseCookie("Key=", true);
  testParseCookie("SPACE%20=SPACE%20", true);
  testParseCookie("Key=\"\"", true);

  testParseCookie("", false);
  testParseCookie("=Value", false);
  testParseCookie("key", false);
  testParseCookie("key=value; ", false);
  testParseCookie("\"key\"=value", false);
  testParseCookie("Key=\"Val\"ue", false);
  testParseCookie("Key=\"Value\"\"\"", false);
  testParseCookie("Key=VALUE\"1\"", false);
  testParseCookie("Key=V,A\\LUE", false);
}

TEST(CookieManager, EXPIRES) {
  testParseCookie("k=v; Expires=Wed, 09 Jun 2021 10:18:14 GMT", true);
  testParseCookie("k=v; Expires=Mon, 00 Jun 0000 00:00:00 GMT", true);
  testParseCookie("k=v; Expires=Sun, 99 Dec 9999 99:99:99 GMT", true);

  testParseCookie("k=v; Expires=Wed, 09 Jun 2021 10:18:14 GMT ", false);
  testParseCookie("k=v; Expires=Wed, 09 Jun 2021 10:18:14  GMT", false);
  testParseCookie("k=v; Expires=wed, 09 jun 2021 10:18:14 GMT", false);
  testParseCookie("k=v; Expires=ed, 09 Jun 2021 10:18:14 GMT", false);
  testParseCookie("k=v; Expires=Wed, 09 JJJun 2021 10:18:14 GMT", false);
  testParseCookie("k=v; Expires=", false);
}

TEST(CookieManager, MAX_AGE) {
  testParseCookie("k=v; Max-Age=12", true);
  testParseCookie("k=v; Max-Age=-1", true);
  testParseCookie("k=v; Max-Age=-0", true);

  testParseCookie("k=v; Max-Age=", false);
  testParseCookie("k=v; Max-Age=abc", false);
  testParseCookie("k=v; Max-Age=ABC", false);
  testParseCookie("k=v; Max-Age=0", false);
  testParseCookie("k=v; Max-Age=0123", false);
  testParseCookie("k=v; max-age=\n", false);
}

TEST(CookieManager, DOMAIN) {
  testParseCookie("k=v; Domain=www.google.com", true);
  testParseCookie("k=v; Domain=localhost", true);
  testParseCookie("k=v; Domain=127.0.0.1", true);
  testParseCookie("k=v; Domain=0.0.0.0", true);

  testParseCookie("k=v; Domain=", false);
  testParseCookie("k=v; Domain=localhost:80", false);
  testParseCookie("k=v; Domain=google%20.%20com", false);
}

TEST(CookieManager, PATH) {
  testParseCookie("k=v; Path=/", true);
  testParseCookie("k=v; Path=/app/", true);
  testParseCookie("k=v; Path=app/", true);
  testParseCookie("k=v; Path=./app", true);
  testParseCookie("k=v; Path=../app", true);
  testParseCookie("k=v; Path=/%20abc%20/", true);

  testParseCookie("k=v; Path=", false);
  testParseCookie("k=v; Path=\a", false);
  testParseCookie("k=v; Path=\n", false);
}

TEST(CookieManager, SECURE_AND_HTTPONLY) {
  testParseCookie("k=v; Secure", true);
  testParseCookie("k=v; HttpOnly", true);
  testParseCookie("k=v; Secure; HttpOnly", true);
  testParseCookie("k=v; HttpOnly; Secure", true);
  testParseCookie("A=b; sECURE; SeCuRe; HtTpOnLy; hTtPoNlY", true);

  testParseCookie("k=v; Secure=", false);
  testParseCookie("k=v; HttpOnly=", false);
  testParseCookie("k=v; Secure=true", false);
  testParseCookie("k=v; HttpOnly=true", false);
  testParseCookie("k=v; Secure=false", false);
  testParseCookie("k=v; HttpOnly=false", false);
}

TEST(CookieManager, MIX_ATTR) {
  testParseCookie("A=b; Expires=Sun, 06 Nov 1994 08:49:37 GMT; Max-Age=2",
                  true);
  testParseCookie("A=b; HttpOnly; Max-Age=12", true);
  testParseCookie("A=b; sECURE; pAtH=/; DoMaIn=localhost", true);
  testParseCookie("A=b; Domain=0.0.0.0; Domain=1.1.1.1", true);
  testParseCookie("A=b; Domain=1.1.1.1; max-age=-99999999", true);
  testParseCookie("Max-Age=2; Domain=1.1.1.1; max-age=-99999999", true);
  testParseCookie("KEY=\"VALUE\"; Domain=1.1.1.1", true);
}

TEST(CookieManager, COOKIE_MANAGER) {
  testCookieManager({"k=v"}, "k", "k=v");
  testCookieManager({"k=v"}, "NONO", "");
  // Cookie属性が重複する場合、最後の値を設定
  testCookieManager({"k=v; Path=/; Path=/app/; Path=/var/"}, "k",
                    "k=v; Path=/var/");
  // Cookieが重複する場合は最後のCookieを設定
  testCookieManager({"k=v", "k=v2"}, "k", "k=v2");
  testCookieManager({"k=v", "k=\"v2\""}, "k", "k=\"v2\"");
  // Cookie属性を出力時はUpperCamelCase
  testCookieManager({"k=v; secure; SECURE; SeCuRe"}, "k", "k=v; Secure");
  // Cookieが不正な場合は無視される
  testCookieManager({"k=v", "k=;;;;"}, "k", "k=v");
  // Cookie属性が不正な場合は無視される
  testCookieManager({"k=v", "k=v2; Max-Age=abc"}, "k", "k=v");
  testCookieManager({"k=v", "k=v2; NO-EXIST-ATTER"}, "k", "k=v");
  testCookieManager({"k=v", "k=v2; NO-EXIST-ATTER", "k=v3"}, "k", "k=v3");
}
