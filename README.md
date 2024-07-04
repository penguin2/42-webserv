# 42-webserv

## プロジェクト概要
* nginxライクなHTTPサーバの実装
* CGIの実装
* Cookieを用いたセッションを含むWebサイトの実装

## 起動方法
1. `git clone https://github.com/penguin2/42-webserv.git`
2. `cd 42-webserv`
3. `make`
4. `./webserv [/path/to/configファイル]` 引数無し実行でサンプルサイトを表示([コンフィグファイルの詳細な設定方法](https://github.com/penguin2/42-webserv/wiki/Nginx-config))

## ファイル構成
```
.
├── Makefile
├── README.md
├── config_sample
├── inc
│   ├── config
│   └── utils
├── map
├── sample
│   ├── 42library
│   │   ├── 42library
│   │   │   ├── business_logic
│   │   │   ├── persistence
│   │   │   ├── presentation
│   │   ├── html
│   │   ├── images
│   │   └── static
│   ├── assets
│   ├── cgi_ext
│   ├── index.html
│   └── upload
├── srcs
│   ├── cgi
│   ├── config
│   ├── cookie
│   ├── event
│   ├── http
│   ├── httpmock
│   ├── log
│   ├── main.cpp
│   ├── request
│   ├── request_handler
│   ├── response
│   ├── server
│   ├── socket
│   ├── test_main
│   ├── time
│   ├── unit_test
│   └── utils
├── test
│   ├── config_test
│   ├── request_parse_test
│   ├── stress_test
│   │   ├── html
│   │   ├── run.sh
│   │   └── tests
│   │       ├── cgi
│   │       ├── get
│   │       └── post
│   └── system_test
│       ├── requirements.txt
│       ├── run.sh
│       ├── system_test_utils
│       └── tests
│           ├── allow_methods
│           ├── body_limit
│           ├── cgi_error_timeout
│           ├── cgi_general
│           ├── cgi_keep_alive
│           ├── cgi_large_size_body
│           ├── cgi_mulitple
│           ├── cgi_response_4pattern
│           ├── chunk
│           ├── delete
│           ├── error_page
│           ├── get
│           ├── keep_alive
│           ├── limit_uri_header_size
│           ├── multiple_cgi_request
│           ├── multiple_request
│           ├── multiple_server
│           ├── no_match_location
│           ├── pipe_broken
│           ├── post
│           ├── redirect
│           ├── timeout
│           └── upload
└── webserv.conf
```

## configファイル
```
http {
    server {
        # ポートのみ記述・IPアドレスとポート両方記述の2種類
        # listen 127.0.0.1:4242;
        listen 80;

        # ホスト名
        server_name example.com;

        # エラーの場合に表示させるページ(複数ステータスコードを指定可能)
        error_page 404 /404.html;
        error_page 500 502 503 504 /50x.html;

        location / {
            # 記述しない場合は全てのメソッドを許可
            allow_methods GET POST;
            # ベースとなるディレクトリを指定、相対パスと絶対パスの両方指定可能
            root /var/www/html;
            # ディレクトリに対してリクエストされた際に仕様するデフォルトのファイルを指定
            index index.html;
            # ディレクトリに対してリクエストされた際にディレクトリリストを表示
            autoindex on;
        }

        location /route1 {
            # リダイレクトを定義(301,302,303,307,308のみ)
            return 301 /new-location;
        }

        location /route2 {
            root /var/www/example;
        }

        location /upload {
            allow_methods POST;
            # アップロードされたファイルサイズの上限(メガバイト単位)
            client_max_body_size 100M;
            # POSTメソッドによるリクエストをファイルアップロードとするか
            upload on;
        }
        location /cgi-bin {
            allow_method GET POST;
            # CGIスクリプトの拡張子を指定
            cgi_ext .py .php;
        }
    }
}
```
