from typing import Union
from system_test_utils.get_status_code import get_status_code_and_connection_from_response
import socket
from concurrent.futures import ThreadPoolExecutor

# "connection: close" があると必ずConnectionを切断
CONN_HEADERS = [
    {"connection": "Keep-Alive"},
    {"connection": "XXX"},
    {},
    {"connection": "close"}
]
# Connectionを維持する場合
KEEP_ALIVE_PATTERN = [True, True, True, False]

# ConnectionをCloseする場合
CLOSE_PATTERN = [False, False, False, False]


def make_request_string(method: str,
                        path: str,
                        headers: dict[str, str],
                        host: str = "localhost") -> str:
    request = method + " " + path + " HTTP/1.1\r\n"
    request += "Host:" + host + "\r\n"
    for key, value in headers.items():
        request += key + ": " + value + "\r\n"
    request += "\r\n"
    return request


def send_same_request_twice(request: str,
                            host: str = "localhost",
                            port: int = 4242) -> list[str]:
    responses: list[str] = list()
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, port))
            s.send(request.encode())
            responses.append(s.recv(10000).decode())
            s.send(request.encode())
            responses.append(s.recv(10000).decode())
    except ConnectionResetError:
        responses.append("")

    return responses


def check_keep_alive(first_response: str,
                     second_response: str,
                     expect_status_code: int,
                     expect_keep_alive: bool):
    code, conn = get_status_code_and_connection_from_response(first_response)
    assert code == expect_status_code
    # 2回目のResponseがempty == Connectionが切れている
    if len(second_response) == 0:
        assert not expect_keep_alive
        assert conn.lower() == "close"
    else:
        assert expect_keep_alive
        assert conn.lower() == "keep-alive"


def check_keep_alive_with_various_conn_headers(method: str,
                                               path_or_path_list: Union[str, list[str]],
                                               expect_code: int,
                                               expect_keepalive_pattern: bool):
    """
    引数 path_or_path_list にstr型(path)とlist[str]型(path_list)を受け付ける
    if (str型(path)):
        paths = [path, path, path, path]
    elif (list[str]型(path_list)):
        paths = path_list
    """
    if isinstance(path_or_path_list, str):
        paths = [path_or_path_list for _ in range(len(CONN_HEADERS))]
    else:
        paths = path_or_path_list

    requests = [
        make_request_string(method, path, conn_header)
        for path, conn_header in zip(paths, CONN_HEADERS)
    ]

    with ThreadPoolExecutor(max_workers=len(CONN_HEADERS)) as executor:
        responses_list = executor.map(send_same_request_twice, requests)

    expect_keepalives = KEEP_ALIVE_PATTERN if expect_keepalive_pattern else CLOSE_PATTERN

    for responses, expect_keepalive in zip(responses_list, expect_keepalives):
        first_res = responses[0]
        second_res = responses[1]
        check_keep_alive(first_res, second_res, expect_code, expect_keepalive)
