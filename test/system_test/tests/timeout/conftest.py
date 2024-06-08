import pytest
import time
from concurrent.futures import ThreadPoolExecutor
import socket
import os
import shutil

HOST = "localhost"
PORT = 4242
REQUEST_DATA = b"GET /index.html HTTP/1.1\r\nHost:1\r\n\r\n"
HTML_DIR = "./html"


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    前処理で必要なディレクトリとファイルの作成
    後処理でディレクトリの削除
    """
    os.mkdir(HTML_DIR)
    with open(HTML_DIR + "/index.html", "w") as f:
        f.write("INDEX")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def get_status_code_or_None(response_data: bytes):
    """
    if (Connection disconnected) -> None
    else -> status_code
    """
    if len(response_data) == 0:
        return None
    response_lines = response_data.decode().split("\r\n")
    status_line = response_lines[0]
    status_code = int(status_line.split()[1])
    return status_code


def request_by_get(sleep_time: int):
    """
    サーバとコネクション確立後、sleep_time秒間遅延させる
    受け取ったレスポンスのByte数が0ならNoneを返す
    そうでないならレスポンスのステータスコードを返す
    """
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            time.sleep(sleep_time)
            s.send(REQUEST_DATA)
            response_data: bytes = s.recv(10000)
            return get_status_code_or_None(response_data)
    except Exception:
        return None


def request_by_thread_pool(sleep_times: list[float], expect_codes: list):
    """
    スレッドプールをテスト数分作成し並列に走らせる
    """
    with ThreadPoolExecutor(max_workers=len(expect_codes)) as executor:
        codes = executor.map(request_by_get, sleep_times)

    for status_code, expect_status_code in zip(codes, expect_codes):
        assert status_code == expect_status_code
