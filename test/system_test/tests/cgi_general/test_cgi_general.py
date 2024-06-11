from conftest import check_cgi_meta_data


def _wrap_check_cgi_meta_data_by_all_methods(
        path: str,
        request_body: str = "",
        headers: dict[str, str] = {}):
    check_cgi_meta_data("get", path, request_body, headers)
    check_cgi_meta_data("post", path, request_body, headers)
    check_cgi_meta_data("delete", path, request_body, headers)


def test_cgi_meta_vars():
    check_cgi_meta_data("get", "/meta_var_get.py")
    check_cgi_meta_data("post", "/meta_var_post.py")
    check_cgi_meta_data("delete", "/meta_var_delete.py")


def test_query_string():
    _wrap_check_cgi_meta_data_by_all_methods("/query_string1.py?q=50")
    _wrap_check_cgi_meta_data_by_all_methods("/query_string2.py?space=%20")
    _wrap_check_cgi_meta_data_by_all_methods("/query_string2.py?space= ")


def test_path_info_and_translated():
    _wrap_check_cgi_meta_data_by_all_methods("/path_info.py/abc")
    _wrap_check_cgi_meta_data_by_all_methods("/path_info_and_query.py/123?p=1")


def test_cgi_body():
    headers = {"Content-length": "4", "content-TYPE": "text/html"}
    _wrap_check_cgi_meta_data_by_all_methods(
        "/check_body.py", request_body="BODY", headers=headers)


def test_current_dir():
    _wrap_check_cgi_meta_data_by_all_methods("/check_current_dir.py")


def test_internal_redirect():
    headers = {"Content-length": "4", "content-TYPE": "text/html"}
    check_cgi_meta_data("get", "/", "BODY", headers)


def test_http_headers():
    headers = {
        "my-header1": "ABC",
        "My_headeR2": "DEF",
        "HTTP_MY_HEADER3": ""
    }
    _wrap_check_cgi_meta_data_by_all_methods(
        "/http_headers.py", headers=headers)
