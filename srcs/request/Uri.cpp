#include "Uri.hpp"

#include "HttpException.hpp"
#include "utils/UriUtils.hpp"
#include "utils/Utils.hpp"

Uri::Uri(void) {}
Uri::~Uri(void) {}

void Uri::overwriteAuthorityIfNotSet(const std::string& authority) {
  if (this->host_.size() == 0) parseAuthority(authority);
}

void Uri::overwritePath(const std::string& new_path) {
  this->setAndCheckAndDecodePath(new_path);
}

void Uri::parse(const std::string& uri) {
  // origin-form(原形式)
  // AuthorityはHostヘッダを読み取ったらをセットする
  if (uri[0] == '/') {
    // Schemeはデフォルトのhttpをセットする
    setAndCheckScheme("http");
    parsePathQueryFragment(uri);
  }
  // absolute-form(絶対形式)
  else {
    const size_t scheme_end_idx = uri.find("://");
    // Schemeが空
    if (scheme_end_idx == std::string::npos)
      throw HttpException(HttpException::BAD_REQUEST, "Bad URI Format");
    setAndCheckScheme(uri.substr(0, scheme_end_idx));
    // strlen("://") == 3
    const size_t authority_start_idx = (scheme_end_idx + 3);
    const size_t authority_end_idx =
        uri.find_first_of("/#?", authority_start_idx);
    if (authority_end_idx == std::string::npos) {
      // Authorityの終端がヌル文字のためPath等は無し
      parseAuthority(uri.substr(authority_start_idx));
    } else {
      // AuthorityAutorityの終端が"/#?"のためそれに続くPath等がある
      const size_t authority_size = authority_end_idx - authority_start_idx;
      parseAuthority(uri.substr(authority_start_idx, authority_size));
      parsePathQueryFragment(uri.substr(authority_start_idx + authority_size));
    }
  }
}

void Uri::parseAuthority(const std::string& authority) {
  const size_t user_info_size = authority.find('@');
  size_t host_start_idx = 0;
  // Schemeと'@'の間はUserInfo
  if (user_info_size != std::string::npos) {
    setAndCheckAndDecodeUserInfo(authority.substr(0, user_info_size));
    host_start_idx = user_info_size + 1;
  }
  // UserInfoに:が含まれる可能性があるためhost_start_idx以降から探す
  const size_t host_end_idx = authority.find(':', host_start_idx);
  // portが明示的に指定されていない場合defaultで80番を設定
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
  const size_t fragment_idx = path_query_fragment.find('#');
  if (fragment_idx != std::string::npos)
    setFragment(path_query_fragment.substr(fragment_idx + 1));
  const std::string path_query = path_query_fragment.substr(0, fragment_idx);
  const size_t query_idx = path_query.find('?');
  if (query_idx != std::string::npos)
    setAndCheckQuery(path_query.substr(query_idx + 1));
  setAndCheckAndDecodePath(path_query.substr(0, query_idx));
}

const std::string& Uri::getScheme(void) const { return scheme_; }
const std::string& Uri::getUserInfo(void) const { return user_info_; }
const std::string& Uri::getHost(void) const { return host_; }
size_t Uri::getPort(void) const { return port_; }
const std::string& Uri::getPath(void) const { return path_; }
const std::string& Uri::getQuery(void) const { return query_; }
const std::string& Uri::getFragment(void) const { return fragment_; }

void Uri::setAndCheckScheme(const std::string& scheme) {
  // Schemeは大文字小文字を区別しない
  this->scheme_ = utils::toLower(scheme);
  if (this->scheme_ != "http")
    throw HttpException(HttpException::MISDIRECTED_REQUEST, "Scheme != http");
}

void Uri::setAndCheckAndDecodeUserInfo(const std::string& user_info) {
  this->user_info_ = user_info;
  // (使用不可文字を含む or デコードに失敗)
  if (!utils::isContainsOnly(user_info_, uri_utils::isUserInfoCharset) ||
      uri_utils::decodeUrlEncoding(this->user_info_) == false)
    throw HttpException(HttpException::BAD_REQUEST, "Bad UserInfo");
}

void Uri::setAndCheckAndDecodeHost(const std::string& host) {
  // Hostは大文字小文字を区別しない
  this->host_ = utils::toLower(host);
  // Hostコンポーネントが空
  if (this->host_.size() == 0)
    throw HttpException(HttpException::BAD_REQUEST, "Empty Host");
  if (!utils::isContainsOnly(this->host_, uri_utils::isRegName) ||
      uri_utils::decodeUrlEncoding(this->host_) == false)
    throw HttpException(HttpException::BAD_REQUEST, "Bad Host");
}

void Uri::setAndCheckPort(const std::string& port) {
  size_t num;
  // "http://localhost:/"のような場合にdefaultのポート番号を設定
  if (port.size() == 0) {
    this->port_ = 80;
  } else {
    if (utils::strToSize_t(port, num, 10) == false)
      throw HttpException(HttpException::BAD_REQUEST, "Bad Port");
    this->port_ = num;
  }
}

void Uri::setAndCheckAndDecodePath(const std::string& path) {
  // ".." "." "//"を整形し親ディレクトリを参照する場合、HttpExceptionをthrow
  this->path_ = uri_utils::removeDotSegments(path);
  // (使用不可文字を含む or デコードに失敗)
  if (!utils::isContainsOnly(this->path_, uri_utils::isPathCharset) ||
      uri_utils::decodeUrlEncoding(this->path_) == false)
    throw HttpException(HttpException::BAD_REQUEST, "Bad Path");
}

void Uri::setAndCheckQuery(const std::string& query) {
  this->query_ = query;
  std::string cpy_query(query);
  // (使用不可文字を含む or デコードに失敗)
  if (!utils::isContainsOnly(this->query_, uri_utils::isQueryCharset) ||
      uri_utils::decodeUrlEncoding(cpy_query) == false)
    throw HttpException(HttpException::BAD_REQUEST, "Bad Query");
}

void Uri::setFragment(const std::string& fragment) {
  // fragmentはURI上には使用可能文字等の定義があるが
  // サーバではfragmentは一律に無視をするため処理は特にしない
  this->fragment_ = fragment;
}

std::string Uri::buildAbsoluteUri(void) const {
  std::stringstream ss;

  ss << scheme_ << "://" << buildAuthority() << path_;
  if (query_.empty() == false) ss << "?" << query_;
  return ss.str();
}

std::string Uri::buildAuthority(void) const {
  std::stringstream ss;

  if (user_info_.empty() == false) ss << user_info_ << '@';
  ss << host_ << ':' << port_;
  return ss.str();
}
