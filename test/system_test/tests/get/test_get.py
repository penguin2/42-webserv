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
