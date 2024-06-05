from conftest import request_by_get

URL_EXIST_ERROR_PAGE = "http://127.0.0.1:4242"
URL_NONE_ERROR_PAGE = "http://127.0.0.1:1234"
URL_INVALID_ERROR_PAGE = "http://127.0.0.1:8888"
URL_DIR_ERROR_PAGE = "http://127.0.0.1:9999"


def test_exist_error_page_directive():
    # [ EXIST_DIRECTIVE | REQUEST_SUCCESS | STATUS_CODE_MACTH_ERROR_PAGE_CODE ]

    # [ EXIST | SUCCESS | ]
    request_by_get(URL_EXIST_ERROR_PAGE + "/index.html", 200, False, "INDEX")
    # [ EXIST | ERROR(404) | YES ]
    # error_pageディレクティブで指定したカスタムエラーページがレスポンスされる
    request_by_get(URL_EXIST_ERROR_PAGE + "/NONO", 404, False, "404")
    # [ EXIST | ERROR(405) | NO ]
    request_by_get(URL_EXIST_ERROR_PAGE + "/get_not_allow/", 405, True)


def test_exist_error_page_directive_and_invalid_error_page_path():
    # [ EXIST_DIRECTIVE | REQUEST_SUCCESS | STATUS_CODE_MACTH_ERROR_PAGE_CODE ]

    # [ EXIST | SUCCESS | ]
    request_by_get(URL_INVALID_ERROR_PAGE + "/index.html", 200, False, "INDEX")
    # [ EXIST | ERROR(404) | YES ]
    # error_page_pathが不正な値のためdefault_error_pageをレスポンス
    request_by_get(URL_INVALID_ERROR_PAGE + "/NONO", 404, True)
    # [ EXIST | ERROR(405) | NO ]
    request_by_get(URL_INVALID_ERROR_PAGE + "/get_not_allow/", 405, True)


def test_exist_error_page_directive_and_error_page_path_is_directory():
    # [ EXIST_DIRECTIVE | REQUEST_SUCCESS | STATUS_CODE_MACTH_ERROR_PAGE_CODE ]

    # [ EXIST | SUCCESS | ]
    request_by_get(URL_DIR_ERROR_PAGE + "/index.html", 200, False, "INDEX")
    # [ EXIST | ERROR(404) | YES]
    # error_page_pathがfileでなくdirectoryのためdefault_error_pageをレスポンス
    request_by_get(URL_DIR_ERROR_PAGE + "/NONO", 404, True)
    # [ EXIST | ERROR(405) | NO ]
    request_by_get(URL_DIR_ERROR_PAGE + "/get_not_allow/", 405, True)


def test_none_error_page_directive():
    # [ EXIST_DIRECTIVE | REQUEST_SUCCESS | STATUS_CODE_MACTH_ERROR_PAGE_CODE ]

    # [ NONE | SUCCESS | ]
    request_by_get(URL_NONE_ERROR_PAGE + "/index.html", 200, False, "INDEX")
    # [ NONE | ERROR(404) | NO ]
    request_by_get(URL_NONE_ERROR_PAGE + "/NONO", 404, True)
