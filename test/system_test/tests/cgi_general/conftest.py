from utils.shebang import write_shebang_to_files
from utils.shebang import remove_shebang_from_files
from utils.retry_on_exception import retry_on_exception

import requests
import pytest
import glob


CGI_BIN_DIR = "./cgi-bin"
BASE_URL = "http://localhost:4242/cgi-bin"
PYTHON = "python3"


@pytest.fixture(scope="session", autouse=True)
def manage_shebang():
    """
    テスト前に実行環境に合わせたシェバンをCGIスクリプトに記述
    テスト後にシェバンを削除
    """
    files = glob.glob(CGI_BIN_DIR + "/*")
    write_shebang_to_files(files, PYTHON)
    yield
    remove_shebang_from_files(files)


@retry_on_exception
def check_cgi_meta_data(method_lower_str: str,
                        path: str,
                        request_body: str = "",
                        headers: dict[str, str] = {}):
    """
    第一引数のmethod文字列からrequestsライブラリのどのメソッドを呼び出すかを決定
    if (CGIスクリプト is SUCCESS) -> 200
    else -> 500(pythonのAssertErrorで500番) or other StatusCodes
    CGIで稀にエラーが発生するのでデコレータによって何回かretryする
    """
    url = BASE_URL + path
    requests_method = getattr(requests, method_lower_str)
    res = requests_method(url, data=request_body, headers=headers)

    assert res.status_code == 200
