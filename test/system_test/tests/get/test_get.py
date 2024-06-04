from conftest import request_by_get, get_file_without_permission


def test_simple_get_success():
    request_by_get("", 200, "VALUE=1")
    request_by_get("/", 200, "VALUE=1")
    request_by_get("/value2.html", 200, "VALUE=2")
    request_by_get("/value%20.html", 200, "VALUE=3")
    request_by_get("/html2/value4.html", 200, "VALUE=4")
    request_by_get("/html2/././value4.html", 200, "VALUE=4")
    request_by_get("/html2/../index.html", 200, "VALUE=1")
    request_by_get("/?abc=10#fragment", 200, "VALUE=1")
    request_by_get("/value2.html#abc", 200, "VALUE=2")
    request_by_get("/value2.html?q=50", 200, "VALUE=2")


def test_simple_get_error():
    request_by_get("/NONO", 404, "ERROR")
    request_by_get("/@abc", 404, "ERROR")
    request_by_get("/abc?@abc", 404, "ERROR")


def test_no_permission():
    get_file_without_permission("/index.html", 404, "ERROR")


def test_autoindex():
    request_by_get("/is_autoindex/", 200)
    request_by_get("/is_not_autoindex/", 404)
    get_file_without_permission("/is_autoindex/", 404, "ERROR")
    get_file_without_permission("/is_not_autoindex/", 404, "ERROR")
    get_file_without_permission("/is_autoindex/abc", 404, "ERROR")
    get_file_without_permission("/is_not_autoindex/def", 404, "ERROR")
    request_by_get("/is_autoindex/abc", 200, "VALUE=abc")
    request_by_get("/is_not_autoindex/def", 200, "VALUE=def")


def test_request_to_indexhtml_dir():
    """
    indexディレクティブにindex.htmlを記述
    ./index/index.htmlというディレクトリを配置
    内部redirectするがindex.htmlがディレクトリなので失敗
    NotFound(404)がレスポンスされる
    """
    request_by_get("/index/", 404, "ERROR")


def test_request_to_indexhtml_dir_with_autoindex():
    """
    indexディレクティブにindex.htmlを記述
    autoindexディレクティブにonを記述
    ./index_with_autoindex/index.htmlというディレクトリを配置
    内部redirectするがindex.htmlがディレクトリなので失敗
    autoindexがonなのでautoindex_page(200)がレスポンスされる
    """
    request_by_get("/index_with_autoindex/", 200)


def test_index_with_allowmethods():
    """
    indexディレクティブにindex.htmlを記述
    allow_methodsディレクティブにPOSTを記述
    ./index_with_autoindex/index.htmlというファイルをを配置
    内部redirectするがallow_methodsでGETが制限される
    NotAllow(405)がレスポンスされる
    """
    request_by_get("/index_with_allow_methods/", 405)


def test_index_with_redirect():
    """
    /redirect にアクセスされた際にindexディレクティブでtoを指定
    /redirect/to に内部リダイレクト
    returnディレクティブによって
    /index.html にリダイレクトされ
    OK(200)がレスポンスされる
    """
    request_by_get("/redirect", 200, "VALUE=1")
