from conftest import safe_post_request
from conftest import IGNORE_BODY_PY


def _check_pipe_broken(body_size: int, expect_success: bool):
    body = "a" * body_size
    status_code_or_error: int = safe_post_request(IGNORE_BODY_PY, body)

    if expect_success:
        assert status_code_or_error == 200
    else:
        assert status_code_or_error == -1


def test_pipe_broken():
    _check_pipe_broken(1, True)
    _check_pipe_broken(1000, True)
    _check_pipe_broken(1000000, False)
