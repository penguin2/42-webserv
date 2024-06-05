import requests
import pytest
import os
import shutil

SERVER1 = "SERVER1"
SERVER2 = "SERVER2"
SERVER3 = "SERVER3"
SERVER4 = "SERVER4"
SERVER5 = "SERVER5"
SERVER6 = "SERVER6"
SERVER7 = "SERVER7"
SERVER8 = "SERVER8"
SERVER9 = "SERVER9"
SERVER10 = "SERVER10"
SERVER11 = "SERVER11"
HTML_DIR = "./html/"


def mkdir_and_touch(path: str, body: str = ""):
    with open(path, "w") as f:
        f.write(body)


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    テスト開始時にlディレクトリを作成
    テスト終了後にディレクトリを削除
    """
    os.mkdir(HTML_DIR)
    mkdir_and_touch(HTML_DIR + SERVER1, SERVER1)
    mkdir_and_touch(HTML_DIR + SERVER2, SERVER2)
    mkdir_and_touch(HTML_DIR + SERVER3, SERVER3)
    mkdir_and_touch(HTML_DIR + SERVER4, SERVER4)
    mkdir_and_touch(HTML_DIR + SERVER5, SERVER5)
    mkdir_and_touch(HTML_DIR + SERVER6, SERVER6)
    mkdir_and_touch(HTML_DIR + SERVER7, SERVER7)
    mkdir_and_touch(HTML_DIR + SERVER8, SERVER8)
    mkdir_and_touch(HTML_DIR + SERVER9, SERVER9)
    mkdir_and_touch(HTML_DIR + SERVER10, SERVER10)
    mkdir_and_touch(HTML_DIR + SERVER11, SERVER11)
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def request_by_get(url: str, expect_body: str, headers: dict[str, str] = {}):
    if headers != {}:
        res = requests.get(url, headers=headers)
    else:
        res = requests.get(url)
    assert res.text == expect_body
    assert int(res.headers["Content-Length"]) == len(expect_body)
