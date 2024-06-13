from system_test_utils.check_multiple_requests import check_multiple_requests
from system_test_utils.check_multiple_requests import check_multiple_incomplete_requests

REQUEST200 = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n"
REQUEST404 = "GET /NONO HTTP/1.1\r\nHost: localhost\r\n\r\n"


def test_multiple_2_request():
    check_multiple_requests((REQUEST200, 200), (REQUEST200, 200))
    check_multiple_requests((REQUEST200, 200), (REQUEST404, 404))
    check_multiple_requests((REQUEST404, 404), (REQUEST200, None))
    check_multiple_requests((REQUEST404, 404), (REQUEST404, None))


def test_multiple_3_request():
    check_multiple_requests(
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200)
    )
    check_multiple_requests(
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, 404)
    )
    check_multiple_requests(
        (REQUEST200, 200),
        (REQUEST404, 404),
        (REQUEST200, None)
    )
    check_multiple_requests(
        (REQUEST200, 200),
        (REQUEST404, 404),
        (REQUEST404, None)
    )

    check_multiple_requests(
        (REQUEST404, 404),
        (REQUEST200, None),
        (REQUEST200, None)
    )
    check_multiple_requests(
        (REQUEST404, 404),
        (REQUEST200, None),
        (REQUEST404, None)
    )
    check_multiple_requests(
        (REQUEST404, 404),
        (REQUEST404, None),
        (REQUEST200, None)
    )
    check_multiple_requests(
        (REQUEST404, 404),
        (REQUEST404, None),
        (REQUEST404, None)
    )


def test_multiple_incomplete_request():
    # Number Of Requests == 1
    check_multiple_incomplete_requests(0, (REQUEST200, 200))
    check_multiple_incomplete_requests(0, (REQUEST404, 404))
    """
    1つのリクエストを2分割しているので
    前半のリクエストを送った段階ではレスポンスはrecvできない
    そのためNoneとなる
    """
    check_multiple_incomplete_requests(1, (REQUEST200, None))
    check_multiple_incomplete_requests(1, (REQUEST404, None))

    # Number Of Requests == 3
    check_multiple_incomplete_requests(
        1,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200)
    )
    """
    3つのリクエストを2分割しているので
    前半のリクエストを送った段階では1つのレスポンスしかrecvできない
    """
    check_multiple_incomplete_requests(
        2,
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, 200)
    )
    check_multiple_incomplete_requests(
        3,
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, None)
    )

    # Number Of Requests == 5
    check_multiple_incomplete_requests(
        1,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, 404)
    )
    check_multiple_incomplete_requests(
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
    check_multiple_incomplete_requests(
        3,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, 200),
        (REQUEST404, 200)
    )
    check_multiple_incomplete_requests(
        3,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST404, None),
        (REQUEST404, 404),
        (REQUEST404, None)
    )
    check_multiple_incomplete_requests(
        4,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, None),
        (REQUEST404, 200)
    )
    check_multiple_incomplete_requests(
        5,
        (REQUEST200, 200),
        (REQUEST200, 200),
        (REQUEST200, None),
        (REQUEST200, None),
        (REQUEST404, None)
    )
