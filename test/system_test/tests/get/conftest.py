import requests
import os

BASE_URL = "http://127.0.0.1:4242"
BASE_PATH = "./html/"


def request_by_get(path: str, expect_status_code: int, expect_body: str = ""):
    """
    基本的なGETメソッドの挙動をテスト
    StatusCode・Body・Content-Lengthのdiffをテスト
    BASE_URLは固定にしているのでPathを変えてテストを書いてください
    expect_body引数を省略するとボディの比較は行いません
    """
    url = BASE_URL + path
    res = requests.get(url)
    assert res.status_code == expect_status_code
    if expect_body != "":
        assert res.text == expect_body
        assert int(res.headers["Content-Length"]) == len(expect_body)


def get_file_without_permission(file: str, expect_status_code: int, expect_body: str):
    """
    存在するファイルの権限削除 -> REQUEST -> 権限付与
    """
    file_path = BASE_PATH + file
    os.chmod(file_path, 0o000)
    request_by_get(file, expect_status_code, expect_body)
    os.chmod(file_path, 0o755)
