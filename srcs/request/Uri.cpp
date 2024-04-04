#include "Uri.hpp"

Uri::Uri(void) {}
Uri::~Uri(void) {}

void Uri::parse(const std::string& uri) { path_ = uri; }

const std::string& Uri::getScheme(void) const { return scheme_; }
const std::string& Uri::getUserInfo(void) const { return user_info_; }
const std::string& Uri::getHost(void) const { return host_; }
const std::string& Uri::getPort(void) const { return port_; }
const std::string& Uri::getPath(void) const { return path_; }
const std::string& Uri::getQuery(void) const { return query_; }
const std::string& Uri::getFragment(void) const { return fragment_; }
