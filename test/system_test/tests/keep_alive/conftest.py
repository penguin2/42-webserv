import pytest
import os
import shutil

HOST = "localhost"
PORT = 4242
HTML_DIR = "./html"
POST_DIR = HTML_DIR + "/post"
DELETE_DIR = HTML_DIR + "/delete"


def touch(file: str, body: str = ""):
    with open(file, "w") as f:
        f.write(body)


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    前処理で必要なディレクトリとファイルの作成
    後処理でディレクトリの削除
    """
    os.mkdir(HTML_DIR)
    os.mkdir(POST_DIR)
    os.mkdir(DELETE_DIR)
    os.mkdir(HTML_DIR + "/301")
    os.mkdir(HTML_DIR + "/302")
    os.mkdir(HTML_DIR + "/303")
    os.mkdir(HTML_DIR + "/307")
    os.mkdir(HTML_DIR + "/308")
    touch(HTML_DIR + "/index.html", "INDEX")
    touch(DELETE_DIR + "/index1.html", "INDEX1")
    touch(DELETE_DIR + "/index2.html", "INDEX2")
    touch(DELETE_DIR + "/index3.html", "INDEX3")
    touch(DELETE_DIR + "/index4.html", "INDEX4")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)
