import pytest
import requests
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
HTML_DIR = "./html"
REDIRECT_SUCCESS_DIR = HTML_DIR + "/success"
REDIRECT_ERROR_DIR = HTML_DIR + "/error"


def mkdir_and_touch(directory: str, file: str):
    os.mkdir(directory)
    with open(directory + file, "w"):
        pass


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    前処理で必要なディレクトリの作成
    後処理でディレクトリの削除
    """
    os.mkdir(HTML_DIR)
    mkdir_and_touch(REDIRECT_SUCCESS_DIR, "/index.html")
    mkdir_and_touch(REDIRECT_ERROR_DIR, "/error.html")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def request_with_redirect(path: str, expect_status_codes: list[int]):
    """
    リダイレクトされるURLにリクエスト
    最終的なレスポンスに至るまでのステータスコードを比較
    """
    url = BASE_URL + path
    res = requests.get(url)
    redirect_responses = res.history
    redirect_responses.append(res)
    assert len(redirect_responses) == len(expect_status_codes)
    for red_res, expect_code in zip(redirect_responses, expect_status_codes):
        assert red_res.status_code == expect_code
