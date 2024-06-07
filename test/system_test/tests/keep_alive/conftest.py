import pytest
import socket
import os
import shutil

HOST = "localhost"
PORT = 4242
HTML_DIR = "./html"
POST_DIR = HTML_DIR + "/post"
DELETE_DIR = HTML_DIR + "/delete"


def touch(file: str, body: str = ""):
    with open(file, "w") as f:
        f.write(body)


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    前処理で必要なディレクトリとファイルの作成
    後処理でディレクトリの削除
    """
    os.mkdir(HTML_DIR)
    os.mkdir(POST_DIR)
    os.mkdir(DELETE_DIR)
    os.mkdir(HTML_DIR + "/301")
    os.mkdir(HTML_DIR + "/302")
    os.mkdir(HTML_DIR + "/303")
    os.mkdir(HTML_DIR + "/307")
    os.mkdir(HTML_DIR + "/308")
    touch(HTML_DIR + "/index.html", "INDEX")
    touch(DELETE_DIR + "/index1.html", "INDEX1")
    touch(DELETE_DIR + "/index2.html", "INDEX2")
    touch(DELETE_DIR + "/index3.html", "INDEX3")
    touch(DELETE_DIR + "/index4.html", "INDEX4")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def make_request(method: str, path: str, headers: dict[str, str]) -> str:
    request = method + " " + path + " HTTP/1.1\r\n"
    request += "Host:" + HOST + "\r\n"
    for key, value in headers.items():
        request += key + ": " + value + "\r\n"
    request += "\r\n"
    return request


def get_status_code_and_connection(response_data: bytes) -> tuple[int, str]:
    response_string = response_data.decode()
    response_lines = response_string.split('\r\n')
    status_line = response_lines[0]
    status_code = int(status_line.split()[1])
    for line in response_lines[1:]:
        key, *value = line.split(":")
        if key.lower() == "connection":
            return (status_code, "".join(value).strip())
    return (status_code, "")


def request_by_xxx(method: str, path: str, headers: dict[str, str], expect_status_code: int, expect_keep_alive: bool):
    """
    同じリクエストを2回送信
    実際にコネクションが切れるかと
    レスポンスのConnectionヘッダの値を確認
    """
    request: str = make_request(method, path, headers)
    conn_value = ""

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            # 1回目のREQUESTを送信
            s.connect((HOST, PORT))
            s.send(request.encode())
            response_data: bytes = s.recv(10000)
            status_code, conn_value = get_status_code_and_connection(
                response_data)
            assert status_code == expect_status_code

            # 同じConnectionに2回目のREQUESTを送信
            s.send(request.encode())
            # 受信したデータのサイズでConnectionが維持されているかを確認
            response_data: bytes = s.recv(10000)
            if len(response_data) == 0:
                assert not expect_keep_alive
                assert conn_value.lower() == "close"
            else:
                assert expect_keep_alive
                assert conn_value.lower() == "keep-alive"

    # Connectionが切れる場合はKeepAliveが無効なはず
    except ConnectionResetError:
        assert not expect_keep_alive
        assert conn_value.lower() == "close"
