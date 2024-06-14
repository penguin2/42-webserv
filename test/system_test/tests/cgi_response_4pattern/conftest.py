from system_test_utils.shebang import write_shebang_to_files
from system_test_utils.shebang import remove_shebang_from_files
from system_test_utils.retry_on_exception import retry_on_exception

import requests
import pytest
import glob


CGI_BIN_DIR = "./cgi-bin"
BASE_URL = "http://localhost:4242/cgi-bin/"
PYTHON = "python3"


@pytest.fixture(scope="session", autouse=True)
def manage_shebang():
    """
    テスト前に実行環境に合わせたシェバンをCGIスクリプトに記述
    テスト後にシェバンを削除
    """
    files = glob.glob(CGI_BIN_DIR + "/*.py")
    write_shebang_to_files(files, PYTHON)
    yield
    remove_shebang_from_files(files)


@retry_on_exception
def request_to_cgi(method_lower_str: str,
                   path: str,
                   request_body: str = "",
                   headers: dict[str, str] = {}) -> requests.Response:
    url = BASE_URL + path
    requests_method = getattr(requests, method_lower_str)
    response = requests_method(
        url, allow_redirects=False, data=request_body, headers=headers)
    return response
