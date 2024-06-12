from system_test_utils.shebang import write_shebang_to_files
from system_test_utils.shebang import remove_shebang_from_files

import requests
import pytest
import glob

CGI_BIN_DIR = "cgi-bin/"
BASE_URL = "http://localhost:4242/"
PYTHON = "python3"

@pytest.fixture(scope="session", autouse=True)
def manage_shebang():
    no_need_shebang = [file for file in glob.glob(CGI_BIN_DIR + "*shebang-500.py")]
    need_shebang = [file for file in glob.glob(CGI_BIN_DIR + "*.py") if file not in no_need_shebang]
    write_shebang_to_files(need_shebang, PYTHON)
    yield
    remove_shebang_from_files(need_shebang)

def get_test_scripts(status_code: int, prefix: str):
    return glob.glob(CGI_BIN_DIR + prefix + "*" + str(status_code) + ".py")

def get_response_code_wrapped(testset: tuple[str, str, bool]):
    def get_response_code(method: str, path: str, is_need_body: bool):
        url = BASE_URL + path
        request_method_handler = getattr(requests, method)
        body = "4\n2\n" if is_need_body else ""
        res = request_method_handler(url, data=body, headers={})
        return res.status_code
    method, path, is_need_body = testset
    return get_response_code(method, path, is_need_body)
