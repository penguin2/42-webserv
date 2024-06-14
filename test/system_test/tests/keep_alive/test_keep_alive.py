from system_test_utils.check_keep_alive import check_keep_alive_with_various_conn_headers


def test_keep_alive_get():
    check_keep_alive_with_various_conn_headers("GET", "/index.html", 200, True)
    check_keep_alive_with_various_conn_headers("GET", "/NONO", 404, False)
    check_keep_alive_with_various_conn_headers("GET", "/post", 405, False)


def test_keep_alive_post():
    post_paths = ["/post/a", "/post/b", "/post/c", "/post/d"]
    check_keep_alive_with_various_conn_headers("POST", post_paths, 201, True)
    check_keep_alive_with_various_conn_headers("POST", "/post/a", 409, False)


def test_keep_alive_delete():
    delete_paths = [
        "/delete/index1.html",
        "/delete/index2.html",
        "/delete/index3.html",
        "/delete/index4.html"
    ]
    check_keep_alive_with_various_conn_headers(
        "DELETE", delete_paths, 204, True)
    check_keep_alive_with_various_conn_headers("DELETE", "/NONO", 404, False)


def test_keep_alive_redirect():
    check_keep_alive_with_various_conn_headers("GET", "/301", 301, True)
    check_keep_alive_with_various_conn_headers("GET", "/302", 302, True)
    check_keep_alive_with_various_conn_headers("GET", "/303", 303, True)
    check_keep_alive_with_various_conn_headers("GET", "/307", 307, True)
    check_keep_alive_with_various_conn_headers("GET", "/308", 308, True)
