import requests
import pytest
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
BASE_URL_LIMIT_BODY_SIZE = "http://127.0.0.1:1234"
HTML_DIR = "./html"


@pytest.fixture(scope="session", autouse=True)
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


def post_limit_body_size(path: str, body: str, expect_status_code: int):
    """
    client_max_body_sizeディレクティブでボディを制限したテスト
    """
    url = BASE_URL_LIMIT_BODY_SIZE + path
    res = requests.post(url, data=body)
    assert res.status_code == expect_status_code


def post_to_exist_dir(directory: str, expect_status_code: int):
    """
    存在するディレクトリと同じ名前のファイルを作成するテスト
    """
    new_dir_path = HTML_DIR + directory
    os.mkdir(new_dir_path)
    request_by_post(directory, "exist dir", expect_status_code)


def post_to_dir_without_permission(directory: str, file: str, expect_status_code: int):
    """
    ファイルを作成するディレクトリのパーミッションがない場合のテスト
    """
    new_dir_path = HTML_DIR + directory
    os.mkdir(new_dir_path)
    os.chmod(new_dir_path, 0o000)
    request_by_post(directory + file, "no permission", expect_status_code)
    os.chmod(new_dir_path, 0o755)
