from typing import Optional
from conftest import request_by_get, request_by_divied

REQUEST200 = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n"
REQUEST404 = "GET /NONO HTTP/1.1\r\nHost: localhost\r\n\r\n"


def __wrap_multiple_request(*request_tuples: tuple[str, Optional[int]]):
    """
    引数で複数のタプルを受け取る
    複数のタプルのrequest文字列を全て連結し、1つのrequest文字列にする
    複数のタプルから予想するステータスコードのリストを作成
    """
    request_str = ''.join([request for request, _ in request_tuples])
    expect_status_codes = [expect_code for _, expect_code in request_tuples]
    request_by_get(request_str, expect_status_codes)


def __wrap_multiple_incomplete_request(len_first_recv_res: int, *request_tuples: tuple[str, Optional[int]]):
    """
    引数で前半のリクエストで受け取るレスポンスの数と複数のタプルを受け取る
    複数のタプルのrequest文字列を全て連結し、1つのrequest文字列にする
    複数のタプルから予想するステータスコードのリストを作成
    """
    request_str = ''.join([request for request, _ in request_tuples])
    expect_status_codes = [expect_code for _, expect_code in request_tuples]
    request_by_divied(len_first_recv_res, request_str, expect_status_codes)


def test_multiple_2_request():
    __wrap_multiple_request((REQUEST200, 200), (REQUEST200, 200))
    __wrap_multiple_request((REQUEST200, 200), (REQUEST404, 404))
    __wrap_multiple_request((REQUEST404, 404), (REQUEST200, None))
    __wrap_multiple_request((REQUEST404, 404), (REQUEST404, None))


def test_multiple_3_request():
    __wrap_multiple_request(
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200)
    )
    __wrap_multiple_request(
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, 404)
    )
    __wrap_multiple_request(
        (REQUEST200, 200),
        (REQUEST404, 404),
        (REQUEST200, None)
    )
    __wrap_multiple_request(
        (REQUEST200, 200),
        (REQUEST404, 404),
        (REQUEST404, None)
    )

    __wrap_multiple_request(
        (REQUEST404, 404),
        (REQUEST200, None),
        (REQUEST200, None)
    )
    __wrap_multiple_request(
        (REQUEST404, 404),
        (REQUEST200, None),
        (REQUEST404, None)
    )
    __wrap_multiple_request(
        (REQUEST404, 404),
        (REQUEST404, None),
        (REQUEST200, None)
    )
    __wrap_multiple_request(
        (REQUEST404, 404),
        (REQUEST404, None),
        (REQUEST404, None)
    )


def test_multiple_incomplete_request():
    # Number Of Requests == 1
    __wrap_multiple_incomplete_request(0, (REQUEST200, 200))
    __wrap_multiple_incomplete_request(0, (REQUEST404, 404))
    """
    1つのリクエストを2分割しているので
    前半のリクエストを送った段階ではレスポンスはrecvできない
    そのためNoneとなる
    """
    __wrap_multiple_incomplete_request(1, (REQUEST200, None))
    __wrap_multiple_incomplete_request(1, (REQUEST404, None))

    # Number Of Requests == 3
    __wrap_multiple_incomplete_request(
        1,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200)
    )
    """
    3つのリクエストを2分割しているので
    前半のリクエストを送った段階では1つのレスポンスしかrecvできない
    """
    __wrap_multiple_incomplete_request(
        2,
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, 200)
    )
    __wrap_multiple_incomplete_request(
        3,
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, None)
    )

    # Number Of Requests == 5
    __wrap_multiple_incomplete_request(
        1,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, 404)
    )
    __wrap_multiple_incomplete_request(
        2,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, 404)
    )
    """
    5つのリクエストを2分割しているので
    前半のリクエストを送った段階では2つのレスポンスしかrecvできない
    後半のリクエストを送信した後は3つ目のレスポンスからrecvされる
    """
    __wrap_multiple_incomplete_request(
        3,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, 200),
        (REQUEST404, 200)
    )
    __wrap_multiple_incomplete_request(
        3,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, None),
        (REQUEST404, 404),
        (REQUEST404, None)
    )
    __wrap_multiple_incomplete_request(
        4,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, None),
        (REQUEST404, 200)
    )
    __wrap_multiple_incomplete_request(
        5,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, None),
        (REQUEST404, None)
    )
