from conftest import request_by_xxx

CONN_KEEP = {"connection": "Keep-Alive"}
CONN_XXX = {"connection": "XXX"}
CONN_NONE = {}
CONN_CLOSE = {"connection": "close"}


def __wrap_requests(method: str,
                    path: str,
                    expect_code: int,
                    expect_keepalives: list[bool]):
    """
    リクエストのConnectionヘッダの値がClose -> Connection切断
    それ以外の値 -> Connection維持
    Connectionヘッダが存在しない場合 -> Connection維持
    len(expect_keep_alives) == 4
    1 -> CONN_KEEP
    2 -> CONN_XXX
    3 -> CONN_NONE
    4 -> CONN_CLOSE
    """
    request_by_xxx(method, path, CONN_KEEP, expect_code, expect_keepalives[0])
    request_by_xxx(method, path, CONN_XXX, expect_code, expect_keepalives[1])
    request_by_xxx(method, path, CONN_NONE, expect_code, expect_keepalives[2])
    request_by_xxx(method, path, CONN_CLOSE, expect_code, expect_keepalives[3])


def test_keep_alive_get():
    __wrap_requests("GET", "/index.html", 200, [True, True, True, False])
    __wrap_requests("GET", "/NONO", 404, [False, False, False, False])
    __wrap_requests("GET", "/post", 405, [False, False, False, False])


def test_keep_alive_post():
    request_by_xxx("POST", "/post/a", CONN_KEEP, 201, True)
    request_by_xxx("POST", "/post/b", CONN_XXX, 201, True)
    request_by_xxx("POST", "/post/c", CONN_NONE, 201, True)
    request_by_xxx("POST", "/post/d", CONN_CLOSE, 201, False)
    __wrap_requests("POST", "/post/a", 409, [False, False, False, False])


def test_keep_alive_delete():
    request_by_xxx("DELETE", "/delete/index1.html", CONN_KEEP, 204, True)
    request_by_xxx("DELETE", "/delete/index2.html", CONN_XXX, 204, True)
    request_by_xxx("DELETE", "/delete/index3.html", CONN_NONE, 204, True)
    request_by_xxx("DELETE", "/delete/index4.html", CONN_CLOSE, 204, False)
    __wrap_requests("DELETE", "/NONO", 404, [False, False, False, False])


def test_keep_alive_redirect():
    __wrap_requests("GET", "/301", 301, [True, True, True, False])
    __wrap_requests("GET", "/302", 302, [True, True, True, False])
    __wrap_requests("GET", "/303", 303, [True, True, True, False])
    __wrap_requests("GET", "/307", 307, [True, True, True, False])
    __wrap_requests("GET", "/308", 308, [True, True, True, False])
