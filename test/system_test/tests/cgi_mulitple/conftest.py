from system_test_utils.shebang import write_shebang_to_files
from system_test_utils.shebang import remove_shebang_from_files

import requests
import pytest
import glob

CGI_BIN_DIR = "cgi-bin/"
BASE_URL = "http://localhost:4242/"
PYTHON_BIN = "python3"
PYTHON_EXT = "py"
PHP_BIN = "php"
PHP_EXT = "php"

@pytest.fixture(scope="session", autouse=True)
def manage_shebang():
    cgi_infos = [(PYTHON_BIN, PYTHON_EXT), (PHP_BIN, PHP_EXT)]

    cgi_script_groups = [(glob.glob(CGI_BIN_DIR + "*." + cgi_ext), cgi_bin, cgi_ext) for cgi_bin, cgi_ext in cgi_infos]
    for cgi_scripts, cgi_bin, _ in cgi_script_groups:
        write_shebang_to_files(cgi_scripts, cgi_bin)
    yield
    for cgi_scripts, _, _ in cgi_script_groups:
        remove_shebang_from_files(cgi_scripts)

def get_test_scripts(status_code: int, cgi_ext: str):
    return glob.glob(CGI_BIN_DIR + "*" + str(status_code) + "." + cgi_ext)

def get_response_code(method: str, path: str):
    url = BASE_URL + path
    request_method_handler = getattr(requests, method)
    res = request_method_handler(url, data="", headers={})

    return res.status_code
