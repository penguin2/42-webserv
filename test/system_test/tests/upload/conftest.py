import requests
import pytest
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
HTML_DIR = "./html"


def create_sample_cgi_script(file: str):
    with open(file, "w") as f:
        f.write("#!/bin/python3\n")
        f.write("print('Content-Type: text/html')\n")
        f.write("print('')\n")
        f.write("print('BODY')\n")
    os.chmod(file, 0o777)


def remove_file(path: str):
    if os.path.exists(path):
        os.remove(path)


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    """
    テスト開始時にhtmlディレクトリを作成
    テスト終了後にhtmlディレクトリを削除
    """
    os.mkdir(HTML_DIR)
    os.mkdir(HTML_DIR + "/upload_on")
    os.mkdir(HTML_DIR + "/upload_off")
    os.mkdir(HTML_DIR + "/upload_none")
    upload_on_with_cgi_dir = HTML_DIR + "/upload_on_with_cgi_ext"
    upload_off_with_cgi_dir = HTML_DIR + "/upload_off_with_cgi_ext"
    os.mkdir(upload_on_with_cgi_dir)
    os.mkdir(upload_off_with_cgi_dir)
    create_sample_cgi_script(upload_on_with_cgi_dir + "/sample.py")
    create_sample_cgi_script(upload_off_with_cgi_dir + "/sample.py")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)


def request_by_post(path: str, expect_status_code: int):
    url = BASE_URL + path
    res = requests.post(url, data="INDEX")
    assert res.status_code == expect_status_code
