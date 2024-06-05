import pytest
import requests
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
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
        assert res.headers["Content-Length"] == "0"


def touch_and_delete(file: str, expect_status_code: int):
    """
    ファイルを作成後にDELETEメソッドでファイル削除
    """
    file_path = HTML_DIR + file
    touch(file_path)
    request_by_delete(file, expect_status_code)


def mkdir_and_delete(dir: str, expect_status_code: int):
    os.mkdir(HTML_DIR + dir)
    request_by_delete(dir, expect_status_code)


def delete_file_without_permission(file: str, expect_status_code: int):
    """
    file作成 -> file権限削除 -> REQUEST -> file権限付与
    """
    no_permission_file_path = HTML_DIR + file
    touch(no_permission_file_path)
    os.chmod(no_permission_file_path, 0o000)
    request_by_delete(file, expect_status_code)
    os.chmod(no_permission_file_path, 0o755)


def delete_file_in_dir_without_permission(
        no_permission_dir: str, file: str, expect_status_code: int):
    """
    dir作成 -> file作成 -> dir権限削除 -> REQUEST -> dir権限付与
    """
    no_permission_dir_path = HTML_DIR + no_permission_dir
    os.mkdir(no_permission_dir_path)
    touch(no_permission_dir_path + file)
    os.chmod(no_permission_dir_path, 0o000)
    request_by_delete(no_permission_dir + file, expect_status_code)
    os.chmod(no_permission_dir_path, 0o755)


def touch(file: str):
    with open(file, "w"):
        pass
