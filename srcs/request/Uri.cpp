#include "Uri.hpp"

#include "ServerException.hpp"
#include "UriUtils.hpp"
#include "Utils.hpp"

Uri::Uri(void) {}
Uri::~Uri(void) {}

void Uri::overwriteAuthorityIfNotSet(const std::string& authority) {
  if (this->host_.size() == 0) parseAuthority(authority);
}

void Uri::parse(const std::string& uri) {
  // 原形式
  // Schemeはデフォルトのhttpをセットする
  // AuthorityはHostヘッダを読み取ったらをセットする
  if (uri[0] == '/') {
    setAndCheckScheme("http");
    parsePathQueryFragment(uri);
  }
  // 絶対形式
  else {
    const size_t scheme_end_idx = uri.find("://");
    if (scheme_end_idx == std::string::npos)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Bad URI format");
    setAndCheckScheme(uri.substr(0, scheme_end_idx));
    const size_t authority_start_idx = (scheme_end_idx + 3);
    const size_t authority_end_idx =
        uri.find_first_of("/", authority_start_idx);
    if (authority_end_idx == std::string::npos) {
      parseAuthority(uri.substr(authority_start_idx));
    } else {
      const size_t authority_size = authority_end_idx - authority_start_idx;
      parseAuthority(uri.substr(authority_start_idx, authority_size));
      parsePathQueryFragment(uri.substr(authority_start_idx + authority_size));
    }
  }
}

void Uri::parseAuthority(const std::string& authority) {
  const size_t user_info_size = authority.find('@');
  if (user_info_size != std::string::npos)
    setAndCheckAndDecodeUserInfo(authority.substr(0, user_info_size));
  const size_t host_start_idx =
      (user_info_size != std::string::npos) ? (user_info_size + 1) : 0;
  const size_t host_end_idx = authority.find(':', host_start_idx);
  // portが明示的に指定されていない場合defaultで80番を設定する
  if (host_end_idx == std::string::npos) {
    setAndCheckAndDecodeHost(authority.substr(host_start_idx));
    setAndCheckPort("80");
  } else {
    const size_t host_size = host_end_idx - host_start_idx;
    setAndCheckAndDecodeHost(authority.substr(host_start_idx, host_size));
    setAndCheckPort(authority.substr(host_end_idx + 1));
  }
}

void Uri::parsePathQueryFragment(const std::string& path_query_fragment) {
  size_t delimit_idx = path_query_fragment.find_first_of("?#");
  if (delimit_idx == std::string::npos) {
    setAndCheckAndDecodePath(path_query_fragment);
    return;
  }
  setAndCheckAndDecodePath(path_query_fragment.substr(0, delimit_idx));
  if (path_query_fragment[delimit_idx] == '?') {
    const size_t fragment_start_idx = path_query_fragment.find_first_of("#");
    if (fragment_start_idx == std::string::npos) {
      setAndCheckAndDecodeQuery(path_query_fragment.substr(delimit_idx + 1));
      return;
    }
    const size_t query_size = fragment_start_idx - delimit_idx - 1;
    setAndCheckAndDecodeQuery(
        path_query_fragment.substr(delimit_idx + 1, query_size));
    delimit_idx = fragment_start_idx;
  }
  this->fragment_ = path_query_fragment.substr(delimit_idx + 1);
}

const std::string& Uri::getScheme(void) const { return scheme_; }
const std::string& Uri::getUserInfo(void) const { return user_info_; }
const std::string& Uri::getHost(void) const { return host_; }
size_t Uri::getPort(void) const { return port_; }
const std::string& Uri::getPath(void) const { return path_; }
const std::string& Uri::getQuery(void) const { return query_; }
const std::string& Uri::getFragment(void) const { return fragment_; }

void Uri::setAndCheckScheme(const std::string& scheme) {
  this->scheme_ = scheme;
  Utils::toLowerString(this->scheme_);
  if (scheme != "http")
    throw ServerException(ServerException::SERVER_ERROR_MISDIRECTED_REQUEST,
                          "Scheme != http");
}

void Uri::setAndCheckAndDecodeUserInfo(const std::string& user_info) {
  this->user_info_ = user_info;
  if (Utils::isContainsOnly(this->user_info_, UriUtils::isUserInfoCharset) ==
      false)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad UserInfo");
  UriUtils::decodeUrlEncoding(this->user_info_);
}
void Uri::setAndCheckAndDecodeHost(const std::string& host) {
  this->host_ = host;
  if (this->host_.size() == 0)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Empty Host");
  if (Utils::isContainsOnly(this->host_, UriUtils::isRegName) == false &&
      UriUtils::isIPv4Address(this->host_) == false)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Host");
  UriUtils::decodeUrlEncoding(this->host_);
  Utils::toLowerString(this->host_);
}

void Uri::setAndCheckPort(const std::string& port) {
  size_t num;
  if (port.size() == 0) {
    this->port_ = 80;
  } else {
    if (Utils::strToSize_t(port, num, 10) == false)
      throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                            "Bad Port");
    this->port_ = num;
  }
}

void Uri::setAndCheckAndDecodePath(const std::string& path) {
  this->path_ = path;
  if (Utils::isContainsOnly(this->path_, UriUtils::isPathCharset) == false)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Path");
  UriUtils::decodeUrlEncoding(this->path_);
}

void Uri::setAndCheckAndDecodeQuery(const std::string& query) {
  this->query_ = query;
  if (Utils::isContainsOnly(this->query_, UriUtils::isQueryCharset) == false)
    throw ServerException(ServerException::SERVER_ERROR_BAD_REQUEST,
                          "Bad Query");
  UriUtils::decodeUrlEncoding(this->query_);
}

void Uri::setFragment(const std::string& fragment) { fragment_ = fragment; }
