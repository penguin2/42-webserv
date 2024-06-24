from system_test_utils.request_chunk import request_chunk
import requests
import pytest
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
BASE_PATH = "./html"
BODY_0M = "/0m"
BODY_1M = "/1m"
NO_CLIENT_MAX_BODY_SIZE = "/no_client_max_body_size"


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    os.mkdir(BASE_PATH)
    os.mkdir(BASE_PATH + BODY_0M)
    os.mkdir(BASE_PATH + BODY_1M)
    os.mkdir(BASE_PATH + NO_CLIENT_MAX_BODY_SIZE)
    yield
    if os.path.exists(BASE_PATH):
        shutil.rmtree(BASE_PATH)


def request_normal_body(method: str, path: str, body: str) -> requests.Response:
    url = BASE_URL + path
    headers = {"Content-Length": str(len(body))}
    request_method_handler = getattr(requests, method)
    response = request_method_handler(url, headers=headers, data=body)
    return response


def request_chunked_body(path: str, body: str) -> requests.Response:
    url = BASE_URL + path
    response = request_chunk(url, body)
    return response
