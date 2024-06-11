from conftest import check_cgi_meta_data


def test_cgi_meta_vars():
    check_cgi_meta_data("get", "/meta_var_get.py")
    check_cgi_meta_data("post", "/meta_var_post.py")
    check_cgi_meta_data("delete", "/meta_var_delete.py")


def test_query_string():
    check_cgi_meta_data("get", "/query_string1.py?q=50")
    check_cgi_meta_data("post", "/query_string1.py?q=50")
    check_cgi_meta_data("delete", "/query_string1.py?q=50")

    check_cgi_meta_data("get", "/query_string2.py?space=%20")
    check_cgi_meta_data("get", "/query_string2.py?space= ")


def test_path_info_and_translated():
    check_cgi_meta_data("get", "/path_info.py/abc")
    check_cgi_meta_data("post", "/path_info.py/abc")
    check_cgi_meta_data("delete", "/path_info.py/abc")

    check_cgi_meta_data("get", "/path_info_and_query.py/123?p=1")


def test_cgi_body():
    headers = {"Content-length": "4", "content-TYPE": "text/html"}
    check_cgi_meta_data("get", "/check_body.py", "BODY", headers)
    check_cgi_meta_data("post", "/check_body.py", "BODY", headers)
    check_cgi_meta_data("delete", "/check_body.py", "BODY", headers)


def test_current_dir():
    check_cgi_meta_data("get", "/check_current_dir.py")
    check_cgi_meta_data("post", "/check_current_dir.py")
    check_cgi_meta_data("delete", "/check_current_dir.py")


def test_internal_redirect():
    headers = {"Content-length": "4", "content-TYPE": "text/html"}
    check_cgi_meta_data("get", "/", "BODY", headers)


def test_http_headers():
    headers = {
        "my-header1": "ABC",
        "My_headeR2": "DEF",
        "HTTP_MY_HEADER3": ""
    }
    check_cgi_meta_data("get", "/http_headers.py", headers=headers)
    check_cgi_meta_data("post", "/http_headers.py", headers=headers)
    check_cgi_meta_data("delete", "/http_headers.py", headers=headers)
