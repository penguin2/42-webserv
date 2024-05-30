import requests
import os

BASE_URL = "http://127.0.0.1:4242"


def request_by_get(path: str, expect_status_code: int, expect_body: str):
    """
    基本的なGETメソッドの挙動をテスト
    StatusCode・Body・Content-Lengthのdiffをテスト
    BASE_URLは固定にしているのでPathを変えてテストを書いてください
    """
    url = BASE_URL + path
    res = requests.get(url)
    assert res.status_code == expect_status_code
    assert res.text == expect_body
    assert int(res.headers["Content-Length"]) == len(expect_body)


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
    file_path = "./tests/get/html/index.html"
    os.chmod(file_path, 0o000)
    request_by_get("/index.html", 404, "ERROR")
    os.chmod(file_path, 0o755)
