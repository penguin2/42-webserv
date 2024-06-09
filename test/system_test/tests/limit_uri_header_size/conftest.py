import pytest
import requests
import os
import shutil
import random
import string


BASE_URL = "http://127.0.0.1:4242/"
HTML_DIR = "./html"


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    前処理で必要なディレクトリとファイルの作成
    後処理でディレクトリの削除
    """
    os.mkdir(HTML_DIR)
    with open(HTML_DIR + "/index.html", "w"):
        pass
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def __generate_random_string(string_len: int = 8) -> str:
    alnum = string.ascii_letters + string.digits
    return ''.join(random.choices(alnum, k=string_len))


def generate_random_dict(dict_size: int) -> dict[str, str]:
    random_dict = {}
    while len(random_dict) != dict_size:
        random_dict[__generate_random_string()] = __generate_random_string()
    return random_dict


def request_by_get(path: str, expect_code: int, headers: dict[str, str] = {}):
    url = BASE_URL + path
    res = requests.get(url, headers=headers)
    assert res.status_code == expect_code
