#ifndef WEBSERV_CONFIGADAPTER_H_
#define WEBSERV_CONFIGADAPTER_H_

#include <string>
#include <vector>

#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
namespace ConfigAdapter {
// 該当するServerConfigが無い場合はServerConfigs[0]を使用する
const ServerConfig& searchServerConfig(const std::string& host, size_t port);

// 該当するLocationConfigが無い場合、NULLを返す
const LocationConfig* searchLocationConfig(const ServerConfig& server_conf,
                                           const std::string& path);

// rootディレクティブとURI.Pathからサーバ内の絶対アドレスを作成する
std::string makeAbsolutePath(const LocationConfig& location_conf,
                             const std::string& path);

// (リダイレクト対象のRequest) ? redirect_URI : NULL
const std::string* searchRedirectUri(const LocationConfig& location_conf);

// (リダイレクト対象のRequest) ? redirect_status_code : 0
// searchRedirectUri関数後に実行される想定のため、0が返ることは基本ない
int searchRedirectStatusCode(const LocationConfig& location_conf);

// (Locationにcgi_ext && PathがCGIファイル) ? true : false
bool isCgiPath(const LocationConfig& location_conf, const std::string& path);

// (error_pageに該当するStatusCodeが含まれる) ? error_page_path : NULL
// NULLの場合、呼び出し元でプログラムで組み立てたHTMLを返す
const std::string* searchErrorPage(const ServerConfig& server_conf, int code);

bool isAllowMethods(const LocationConfig& location_conf,
                    const std::string& method);

// 主に405ステータスコードの時にAllowヘッダの値に使用
std::vector<std::string> getAllowMethods(const LocationConfig& location_conf);

// CgiRequestでSERVER_NAMEメタ変数作成時に使用
// 無い場合は空文字列 or デフォルト値？
const std::string* searchHostName(const ServerConfig& server_conf);

bool isAutoindex(const LocationConfig& location_conf);

// indexが存在すればlocationPath + index, なければ空文字列
std::string searchIndex(const LocationConfig& location_conf);

// POSTメソッドで最大値を確認する際に使用
// client_max_body_sizeが設定されていない場合、DEFAULT_MAX_CLIENT_BODY_SIZEを返す
size_t getClientMaxBodySize(const LocationConfig& location_conf);

size_t getMaxNumberOfCrlfBeforeMethod(void);
size_t getMaxMethodSize(void);
size_t getMaxUriSize(void);
size_t getMaxHeaderSize(void);
size_t getMaxNumberOfHeaders(void);
size_t getMaxBodySize(void);

bool isCorrespondingMethod(const std::string& method);

namespace INTERNAL {
const size_t DEFAULT_MAX_URI_SIZE = 2000;
const size_t DEFAULT_MAX_HEADER_SIZE = 200;
const size_t DEFAULT_MAX_NUMBER_OF_HEADERS = 100;
const size_t DEFAULT_MAX_BODY_SIZE = 10000;
const size_t DEFAULT_MAX_CLIENT_BODY_SIZE = 1000;
// (std::string("DELETE").size() == 6)
const size_t DEFAULT_MAX_METHOD_SIZE = 6;
const size_t DEFAULT_MAX_NUMBER_OF_CRLF_BEFORE_METHOD = 10;
}  // namespace INTERNAL

}  // namespace ConfigAdapter

#endif
