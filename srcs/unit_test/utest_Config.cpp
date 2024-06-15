#include <gtest/gtest.h>

#include "config/ConfigParser.hpp"

TEST(Config, MultiDirectiveParse) {
  ConfigParser().parseConfig("./config_sample/OK/sample_multi.conf");

  Config& config = Config::getInstance();
  ServerConfig& server = config.getServer(1);
  EXPECT_EQ(server.getListenPort(), "81");
  EXPECT_EQ(server.getServerName(), "example2.com");
  LocationConfig& location = server.getLocationConfig("/");

  EXPECT_EQ(location.getAllowMethods().size(), 1);
  EXPECT_EQ(location.getAllowMethods()[0], "DELETE");

  EXPECT_EQ(location.getRoot(), "/var/www/html2");

  EXPECT_EQ(location.getIndex(), "index2.html");

  EXPECT_EQ(location.getAutoindex(), false);

  EXPECT_EQ(location.getAllowUpload(), false);

  EXPECT_EQ(location.getReturnStatusCode(), 302);
  EXPECT_EQ(location.getReturnUri(), "/new2-location");

  EXPECT_EQ(location.getClientMaxBodySize(), 1000000);
}
