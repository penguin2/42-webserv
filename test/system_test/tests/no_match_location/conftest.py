import requests

BASE_URL = "http://127.0.0.1:4242"
BASE_PATH = "./html/"


def request_by_get(path: str, expect_status_code: int, expect_body: str = ""):
    url = BASE_URL + path
    res = requests.get(url)
    assert res.status_code == expect_status_code
    if expect_body != "":
        assert res.text == expect_body
