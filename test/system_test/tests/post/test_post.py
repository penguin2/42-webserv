import requests
import pytest
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
BASE_URL_LIMIT_BODY_SIZE = "http://127.0.0.1:1234"
HTML_DIR = "tests/post/html"


@pytest.fixture(scope="module", autouse=True)
def manage_html_directory():
    """
    テスト開始時にhtmlディレクトリを作成
    テスト終了後にhtmlディレクトリを削除
    """
    os.mkdir(HTML_DIR)
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def request_by_post(path: str, body: str, expect_status_code: int):
    """
    基本的なPOSTメソッドの挙動をテスト
    BASE_URLは固定にしているのでPathを変えてテストを書いてください
    """
    url = BASE_URL + path
    res = requests.post(url, data=body)
    assert res.status_code == expect_status_code


def request_limit_body_size(path: str, body: str, expect_status_code: int):
    """
    client_max_body_sizeディレクティブでボディを制限したテスト
    """
    url = BASE_URL_LIMIT_BODY_SIZE + path
    res = requests.post(url, data=body)
    assert res.status_code == expect_status_code


def test_simple_post_success():
    request_by_post("/test1", "abc", 201)
    request_by_post("/test2", "abc", 201)
    request_by_post("/././abc", "abc", 201)


def test_create_file_in_nonexistent_directory():
    """
    存在しないディレクトリにファイルを作成しようとする場合のテスト
    """
    request_by_post("/dir/test2", "nonexist dir", 500)


def test_exist_dir():
    """
    存在するディレクトリと同じ名前のファイルを作成するテスト
    """
    new_dir = "/new_dir"
    new_dir_path = HTML_DIR + new_dir
    os.mkdir(new_dir_path)
    request_by_post(new_dir, "exist dir", 405)


def test_exist_file():
    """
    ファイルを作成
    同じファイルを作成した時のテスト
    """
    request_by_post("/new_file", "abc", 201)
    request_by_post("/new_file", "exist file", 405)


def test_no_permission_dir():
    """
    ファイルを作成するディレクトリのパーミッションがない場合のテスト
    """
    new_dir = "/new_dir_no_permission"
    new_dir_path = HTML_DIR + new_dir
    os.mkdir(new_dir_path)
    os.chmod(new_dir_path, 0o000)
    request_by_post(new_dir + "/new_file", "no permission", 500)
    os.chmod(new_dir_path, 0o755)


def test_client_max_body_size_error():
    request_limit_body_size("/index.html", "client max body limit", 413)
