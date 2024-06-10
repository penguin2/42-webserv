import socket
import re
from typing import Optional
import pytest
import os
import shutil

TIMEOUT_SEC = 0.1

HTML_DIR = "./html/"
HOST = "localhost"
PORT = 4242


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    os.mkdir(HTML_DIR)
    with open(HTML_DIR + "/index.html", "w") as f:
        f.write("INDEX")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def get_status_code_or_None(response_str: Optional[str]) -> Optional[int]:
    """
    if (Connection disconnected) -> None
    else -> status_code
    """
    if response_str is None:
        return None
    if len(response_str) == 0:
        return None
    response_lines = response_str.split("\r\n")
    status_line = response_lines[0]
    status_code = int(status_line.split()[1])
    return status_code


def recv_responses_up_to(number_of_responses: int,
                         s: socket.socket) -> list[Optional[str]]:
    """
    recvしたresponse_strは複数のレスポンスの可能性がある
    response_strを"HTTP/1.1"で区切り、responsesに変換する
    途中でConnectionが切られ、len(recv) == 0となる場合はNoneとなる
    socketにタイムアウトを設定, タイムアウトした場合はNoneとする
    """
    PATTERN = r"(?=HTTP/1.1)"
    responses: list[Optional[str]] = []

    s.settimeout(TIMEOUT_SEC)
    while len(responses) < number_of_responses:
        try:
            response_str: str = s.recv(10000).decode()
            new_responses = re.split(PATTERN, response_str)[1:]
            if len(new_responses) == 0:
                responses.append(None)
            else:
                responses += new_responses

        except socket.timeout:
            responses.append(None)

    return responses


def request_by_get(multiple_request: str,
                   expect_status_codes: list[Optional[int]]):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            s.send(multiple_request.encode())

            responses = recv_responses_up_to(len(expect_status_codes), s)
            status_codes = [get_status_code_or_None(res) for res in responses]

            for code, expect_code in zip(status_codes, expect_status_codes):
                assert code == expect_code

    except ConnectionResetError:
        pass


def request_by_divied(len_first_recv_res: int,
                      multiple_request: str,
                      expect_status_codes: list[Optional[int]]):
    """
    複数のリクエストが連結された multiple_request を2分割する
    前半のリクエストをsend, len_first_recv_resぶんrecv
    後半のリクエストをsend, (総リクエスト数 - len_first_recv_res)ぶんrecv
    リクエストが不完全でレスポンスされるものがないのにrecvした場合
    timeoutし、Noneとして判定する
    """
    request_half_size = int(len(multiple_request) / 2)
    first_half_request = multiple_request[:request_half_size]
    latter_half_request = multiple_request[request_half_size:]
    len_latter_recv_res = len(expect_status_codes) - len_first_recv_res
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            s.send(first_half_request.encode())
            responses = recv_responses_up_to(len_first_recv_res, s)
            s.send(latter_half_request.encode())
            responses += recv_responses_up_to(len_latter_recv_res, s)
            status_codes = [get_status_code_or_None(res) for res in responses]

            for code, expect_code in zip(status_codes, expect_status_codes):
                assert code == expect_code

    except ConnectionResetError:
        pass
