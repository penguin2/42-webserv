from conftest import request_normal_body
from conftest import request_chunked_body
from conftest import BODY_0M, BODY_1M, NO_CLIENT_MAX_BODY_SIZE

ONE_MB = 1000 * 1000
ONE_MB_PLUS_ONE_B = ONE_MB + 1


def _assert_list(list1: list, list2: list):
    for val1, val2 in zip(list1, list2):
        assert val1 == val2


def _check_body_limit_by_normal_body(path: str, body: str, expect_success: bool):
    METHODS = ["post", "get", "delete"]
    SUCCESS_PATTERN = [201, 200, 204]
    ERROR_PATTERN = [413, 413, 413]

    status_codes = [
        request_normal_body(method, path, body).status_code
        for method in METHODS
    ]

    if expect_success:
        _assert_list(status_codes, SUCCESS_PATTERN)
    else:
        _assert_list(status_codes, ERROR_PATTERN)


def _check_body_limit_by_chunked_body(path: str, body: str, expect_status_code: int):
    res = request_chunked_body(path, body)
    assert res.status_code == expect_status_code


def test_body_limit_0mb():
    _check_body_limit_by_normal_body(BODY_0M + "/1", "", True)
    _check_body_limit_by_normal_body(BODY_0M + "/2", 'a' * ONE_MB, False)


def test_body_limit_1m():
    _check_body_limit_by_normal_body(BODY_1M + "/1", "", True)
    _check_body_limit_by_normal_body(BODY_1M + "/2", 'a' * ONE_MB, True)
    _check_body_limit_by_normal_body(
        BODY_1M + "/3", 'a' * ONE_MB_PLUS_ONE_B, False)


def test_body_limit_no_client_max_body_size():
    _check_body_limit_by_normal_body(NO_CLIENT_MAX_BODY_SIZE + "/1", "", True)
    _check_body_limit_by_normal_body(
        NO_CLIENT_MAX_BODY_SIZE + "/2", 'a' * ONE_MB, True)
    _check_body_limit_by_normal_body(NO_CLIENT_MAX_BODY_SIZE + "/3",
                                     'a' * ONE_MB_PLUS_ONE_B, False)


def test_body_limit_0mb_chunk():
    _check_body_limit_by_chunked_body(BODY_0M + "/a", "", 201)
    _check_body_limit_by_chunked_body(BODY_0M + "/b", 'a' * ONE_MB, 413)


def test_body_limit_1m_chunk():
    _check_body_limit_by_chunked_body(BODY_1M + "/a", "", 201)
    _check_body_limit_by_chunked_body(BODY_1M + "/b", 'a' * ONE_MB, 201)
    _check_body_limit_by_chunked_body(
        BODY_1M + "/c", 'a' * ONE_MB_PLUS_ONE_B, 413)


def test_body_limit_no_client_max_body_size_chunk():
    _check_body_limit_by_chunked_body(NO_CLIENT_MAX_BODY_SIZE + "/a", "", 201)
    _check_body_limit_by_chunked_body(
        NO_CLIENT_MAX_BODY_SIZE + "/b", 'a' * ONE_MB, 201)
    _check_body_limit_by_chunked_body(
        NO_CLIENT_MAX_BODY_SIZE + "/c", 'a' * ONE_MB_PLUS_ONE_B, 413)
