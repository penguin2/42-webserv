from typing import Optional
from system_test_utils.retry_on_exception import retry_on_exception
import socket
import re

TIMEOUT_SEC = 0.1


def recv_responses_up_to(
        number_of_responses: int,
        s: socket.socket,
        timeout_sec: float = TIMEOUT_SEC) -> list[Optional[str]]:
    """
    recvしたresponse_strは複数のレスポンスの可能性がある
    response_strを"HTTP/1.1"で区切り、responsesに変換する
    途中でConnectionが切られ、len(recv) == 0となる場合はNone
    socketにタイムアウトを設定, タイムアウトした場合はNone
    """
    PATTERN = r"(?=HTTP/1.1)"
    responses: list[Optional[str]] = []

    s.settimeout(timeout_sec)
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


def request_and_recv_n_responses(request: str,
                                 number_of_responses: int,
                                 host: str = "localhost",
                                 port: int = 4242) -> list[Optional[str]]:
    responses: list[Optional[str]] = []
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.send(request.encode())
        responses = recv_responses_up_to(number_of_responses, s)
    return responses


def divide_and_request(len_first_recv_response: int,
                       len_latter_recv_response: int,
                       request: str,
                       host: str = "localhost",
                       port: int = 4242) -> list[Optional[str]]:
    """
    リクエストを2分割する
    前半のリクエストをsend, len_first_recv_resぶんrecv
    後半のリクエストをsend, len_latter_recv_responseぶんrecv
    リクエストが不完全でレスポンスがない場合timeoutし、Noneとして判定
    """
    responses: list[Optional[str]] = []
    request_half_size = int(len(request) / 2)
    first_half_request = request[:request_half_size]
    latter_half_request = request[request_half_size:]
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.send(first_half_request.encode())
        responses = recv_responses_up_to(len_first_recv_response, s)
        s.send(latter_half_request.encode())
        responses += recv_responses_up_to(len_latter_recv_response, s)
    return responses


@retry_on_exception
def request_and_recv_n_responses_with_retry(
        request: str,
        number_of_responses: int,
        host: str = "localhost",
        port: int = 4242) -> list[Optional[str]]:
    """
    稀に発生するConnectionResetError対策にリトライする
    """
    return request_and_recv_n_responses(request, number_of_responses, host, port)


@retry_on_exception
def divide_and_request_with_retry(len_first_recv_response: int,
                                  len_latter_recv_response: int,
                                  request: str,
                                  host: str = "localhost",
                                  port: int = 4242) -> list[Optional[str]]:
    """
    稀に発生するConnectionResetError対策にリトライする
    """
    return divide_and_request(len_first_recv_response, len_latter_recv_response, request, host, port)
