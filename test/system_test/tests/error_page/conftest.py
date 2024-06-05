import requests
import pytest
import os
import shutil

HTML_DIR = "./html"


def touch(file: str, content: str = ""):
    with open(file, "w") as f:
        f.write(content)


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    テスト開始時にディレクトリを作成
    テスト終了後にディレクトリを削除
    """
    os.mkdir(HTML_DIR)
    touch(HTML_DIR + "/404.html", "404")
    touch(HTML_DIR + "/index.html", "INDEX")
    os.mkdir(HTML_DIR + "/directory")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def request_by_get(url: str,
                   expect_code: int,
                   expect_is_default_error_page: bool,
                   expect_body: str = ""):
    """
    error_pageディレクティブが無い場合はdefault_error_pageをプログラム内で作成
    default_error_pageのbyte数は100Byte以上
    error_pageのsizeが100Bytes未満ならボディの比較も行う
    (error_page_pathからエラーページを組み立てた場合 or リクエストが成功(200 OK)する場合)
    """
    res = requests.get(url)
    assert res.status_code == expect_code
    is_default_error_page = (100 <= len(res.content))
    assert expect_is_default_error_page == is_default_error_page
    if not is_default_error_page:
        assert res.text == expect_body
