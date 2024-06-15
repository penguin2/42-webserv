from system_test_utils.shebang import write_shebang_to_files
from system_test_utils.shebang import remove_shebang_from_files

import requests
import pytest
import glob
import os
import shutil

CGI_BIN_DIR = "cgi-bin/"
BASE_URL = "http://localhost:4242/"
PYTHON = "python3"
UPLOAD_DIR = CGI_BIN_DIR + "upload/"

@pytest.fixture(scope="session", autouse=True)
def manage_shebang():
    need_shebang = glob.glob(CGI_BIN_DIR + "*.py")
    write_shebang_to_files(need_shebang, PYTHON)
    yield
    remove_shebang_from_files(need_shebang)

@pytest.fixture(scope="session", autouse=True)
def make_upload_dir():
    os.mkdir(UPLOAD_DIR)
    yield
    shutil.rmtree(UPLOAD_DIR)

def get_response(method: str, path: str, headers: dict[str, str], body: str):
    url = BASE_URL + path
    request_method_handler = getattr(requests, method)
    res = request_method_handler(url, headers=headers, data=body)
    return (res.status_code, res.content)
