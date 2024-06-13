from system_test_utils.check_keep_alive import check_keep_alive_with_various_conn_headers


def test_cgi_keep_alive():
    check_keep_alive_with_various_conn_headers(
        "GET", "/normal_200.py", 200, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/normal_201.py", 201, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/normal_204.py", 204, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/abnormal_123.py", 123, False)
    check_keep_alive_with_various_conn_headers(
        "GET", "/abnormal_404.py", 404, False)


def test_cgi_keep_alive_has_connection_header():
    check_keep_alive_with_various_conn_headers(
        "GET", "/has_connection_close_200.py", 200, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/has_connection_keep_alive_123.py", 123, False)


def test_cgi_keep_alive_redirect():
    check_keep_alive_with_various_conn_headers(
        "GET", "/redirect_301.py", 301, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/redirect_302.py", 302, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/redirect_303.py", 303, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/redirect_307.py", 307, True)
    check_keep_alive_with_various_conn_headers(
        "GET", "/redirect_308.py", 308, True)
