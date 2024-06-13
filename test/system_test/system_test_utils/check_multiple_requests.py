from system_test_utils.multiple_request import request_and_recv_n_responses_with_retry
from system_test_utils.multiple_request import divide_and_request_with_retry
from system_test_utils.get_status_code import get_status_code_from_response

from typing import Optional


def _assert_list(list1, list2):
    for val1, val2 in zip(list1, list2):
        assert val1 == val2


def check_multiple_requests(*request_tuples: tuple[str, Optional[int]]):
    """
    request_tuple = (REQUEST_STRING, EXPECT_STATUS_CODE_OR_NONE)
    複数のタプルのrequest文字列を全て連結し、1つのrequest文字列にする
    複数のタプルから予想するステータスコードのリストを作成
    """
    request_str = ''.join([request for request, _ in request_tuples])
    expect_status_codes = [expect_code for _, expect_code in request_tuples]

    responses = request_and_recv_n_responses_with_retry(
        request_str, len(expect_status_codes))

    status_codes = [get_status_code_from_response(res) for res in responses]
    _assert_list(status_codes, expect_status_codes)


def check_multiple_incomplete_requests(len_first_recv_res: int,
                                       *request_tuples: tuple[str, Optional[int]]):
    """
    request_tuple = (REQUEST_STRING, EXPECT_STATUS_CODE_OR_NONE)
    引数で前半のリクエストで受け取るレスポンスの数と複数のタプルを受け取る
    複数のタプルのrequest文字列を全て連結し、1つのrequest文字列にする
    複数のタプルから予想するステータスコードのリストを作成
    """
    request_str = ''.join([request for request, _ in request_tuples])
    expect_status_codes = [expect_code for _, expect_code in request_tuples]
    len_latter_recv_res = len(expect_status_codes) - len_first_recv_res

    responses = divide_and_request_with_retry(
        len_first_recv_res, len_latter_recv_res, request_str)

    status_codes = [get_status_code_from_response(res) for res in responses]
    _assert_list(status_codes, expect_status_codes)
