from system_test_utils.create_cgi_script import create_cgi_script

import pytest
import requests
import os
import shutil


CGI_BIN_DIR = "./cgi-bin"
IGNORE_BODY_PY = "/ignore_body.py"
BASE_URL = "http://localhost:4242/cgi-bin"
PYTHON = "python3"

CGI_SCRIPT = """
print("status: 200 OK")
print("Content-Type: text/pre")
print("")
print("SUCCESS")
"""


@pytest.fixture(scope="session", autouse=True)
def manage_cgi_script():
    os.mkdir(CGI_BIN_DIR)
    create_cgi_script(CGI_BIN_DIR + IGNORE_BODY_PY, PYTHON, CGI_SCRIPT)
    yield
    if os.path.exists(CGI_BIN_DIR):
        shutil.rmtree(CGI_BIN_DIR)


def safe_post_request(path: str, body: str) -> int:
    url = BASE_URL + path
    try:
        response = requests.post(url, data=body)
        return response.status_code
    except requests.exceptions.ConnectionError:
        return -1
