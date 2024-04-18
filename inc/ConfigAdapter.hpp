#ifndef WEBSERV_CONFIGADAPTER_H_
#define WEBSERV_CONFIGADAPTER_H_

#include <string>
#include <vector>

namespace ConfigAdapter {

// (リダイレクト対象のRequest) ? redirect_URI : NULL
const std::string* searchRedirectUri(const std::string& host, size_t port,
                                     const std::string& path);

// (リダイレクト対象のRequest) ? redirect_status_code : 0
// searchRedirectUri関数後に実行される想定のため、0が返ることは基本ない
int searchRedirectStatusCode(const std::string& host, size_t port,
                             const std::string& path);

// (Locationにcgi_path&cgi_extがあり、PathがCGIファイル) ? true : false
bool isCgiPath(const std::string& host, size_t port, const std::string& path);

// (error_pageに該当するStatusCodeが含まれる) ? error_page_path : NULL
// NULLの場合、呼び出し元でプログラムで組み立てたHTMLを返す
const std::string* searchErrorPage(const std::string& host, size_t port,
                                   size_t status_code);

// (method in allow_methods) ? true : false
// allow_methodsディレクティブが存在しない場合、全てのMethodが許容される
// allow_methodsディレクティブが存在し、methodが許容されない場合405
bool isAllowMethods(const std::string& host, size_t port,
                    const std::string& path, const std::string& method);

// StatusCode405の時のAllowヘッダの組み立てに使用する予定
std::vector<std::string> getAllowMethods(const std::string& host, size_t port,
                                         const std::string& path);

// 静的ファイルの組み立てをする際に使用
// try_files,root,index,Pathコンポーネントから絶対パスのベクターを組み立てる
std::vector<std::string> makeAbsolutePaths(const std::string& host, size_t port,
                                           const std::string& path);

// Uriの最大サイズ(一般的には2000bytes)
size_t getMaxUriSize(void);
// Headerの最大サイズ(1つのHeaderの最大サイズを想定)
size_t getMaxHeaderSize(void);
// ボディの最大サイズ(client_max_body_sizeが存在しない場合、defaultの値を返す)
size_t getMaxBodySize(const std::string& host, size_t port,
                      const std::string& path);

}  // namespace ConfigAdapter

#endif
