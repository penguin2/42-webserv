import requests
import os
import pytest
import shutil

BASE_URL = "http://127.0.0.1:4242"
HTML_DIR = "tests/delete/html"


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


def request_by_delete(path: str, expect_status_code: int):
    """
    基本的なDELETEメソッドの挙動をテスト
    BASE_URLは固定にしているのでPathを変えてテストを書いてください
    DELETEメソッド成功時にBodyが存在しないことのテスト
    """
    url = BASE_URL + path
    res = requests.delete(url)
    assert res.status_code == expect_status_code
    if res.status_code == 204:
        assert res.text == ""
        assert "Content-Length" not in res.headers


def touch(file: str):
    with open(file, "w"):
        pass


def test_simple_get_success():
    """
    ファイルを作成後にDELETEメソッドでファイル削除
    """
    def touch_and_request_by_delete(file: str, expect_status_code: int):
        file_path = HTML_DIR + file
        touch(file_path)
        request_by_delete(file, expect_status_code)
    touch_and_request_by_delete("/new_file", 204)
    touch_and_request_by_delete("/././abc", 204)


def test_file_not_exist():
    request_by_delete("/no_exist_file", 403)


def test_delete_dir():
    new_dir = "/new_dir"
    os.mkdir(HTML_DIR + new_dir)
    request_by_delete(new_dir, 403)


def test_no_permission_file():
    """
    file作成 -> file権限削除 -> REQUEST -> file権限付与
    """
    no_permission_file = "/no_permission"
    no_permission_file_path = HTML_DIR + no_permission_file
    touch(no_permission_file_path)
    os.chmod(no_permission_file_path, 0o000)
    request_by_delete(no_permission_file, 403)
    os.chmod(no_permission_file_path, 0o755)


def test_no_permission_dir():
    """
    dir作成 -> file作成 -> dir権限削除 -> REQUEST -> dir権限付与
    """
    no_permission_dir = "/no_permission_dir/"
    new_file = "/new_file"
    no_permission_dir_path = HTML_DIR + no_permission_dir
    os.mkdir(no_permission_dir_path)
    touch(no_permission_dir_path + new_file)
    os.chmod(no_permission_dir_path, 0o000)
    request_by_delete(no_permission_dir + new_file, 403)
    os.chmod(no_permission_dir_path, 0o755)
