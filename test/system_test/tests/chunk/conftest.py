from system_test_utils.create_cgi_script import create_cgi_script
from system_test_utils.request_chunk import request_chunk

import pytest
import os
import shutil
import requests

HTML_DIR = "./html/"
CGI_BIN = "cgi-bin/"
BASE_URL = "http://localhost:4242/"
PYTHON_SCRIPT = "echo.py"
PYTHON = "python3"

PYTHON_SCRIPT_TEXT = """
import os
import sys
content_length: int = int(os.environ["CONTENT_LENGTH"])
body: str = sys.stdin.read(10000)
print("Content-Type: text/html")
print("")
print(body, end="")
"""


@pytest.fixture(scope="session", autouse=True)
def manage_directory():
    os.mkdir(HTML_DIR)
    os.mkdir(HTML_DIR + CGI_BIN)
    create_cgi_script(HTML_DIR + CGI_BIN + PYTHON_SCRIPT,
                      PYTHON, PYTHON_SCRIPT_TEXT)
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def check_chunk_upload(path: str, body: str):
    url = BASE_URL + path
    response_upload = request_chunk(url, body)
    response_get = requests.get(url)
    assert response_upload.status_code == 201
    assert response_get.status_code == 200
    assert response_get.text == body


def check_chunk_cgi(path: str, body: str):
    url = BASE_URL + CGI_BIN + path
    response = request_chunk(url, body)
    assert response.text == body
