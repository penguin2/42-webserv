from conftest import request_with_body
from conftest import IGNORE_BODY_PY

import requests


def _check_pipe_broken(body_size: int):
    body = "a" * body_size
    response: requests.Response = request_with_body(IGNORE_BODY_PY, body)
    assert response.status_code == 200
    response: requests.Response = request_with_body(IGNORE_BODY_PY, body)
    assert response.status_code == 200


def test_pipe_broken():
    _check_pipe_broken(1)
    _check_pipe_broken(1000)
    _check_pipe_broken(1000000)
