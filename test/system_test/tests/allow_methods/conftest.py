import requests
import pytest
import os
import shutil

BASE_URL = "http://127.0.0.1:4242"
BASE_PATH = "./html"
GET_PATH = "/get/"
POST_PATH = "/post/"
DELETE_PATH = "/delete/"
GET_DELETE_PATH = "/get_and_delete/"
GET_POST_PATH = "/get_and_post/"
POST_DELETE_PATH = "/post_and_delete/"
ALL_METHODS_PATH = "/all_methods/"
NONE_ALLOW_METHODS_PATH = "/none_allow_methods_directive/"


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    os.mkdir(BASE_PATH)
    os.mkdir(BASE_PATH + GET_PATH)
    os.mkdir(BASE_PATH + POST_PATH)
    os.mkdir(BASE_PATH + DELETE_PATH)
    os.mkdir(BASE_PATH + GET_DELETE_PATH)
    os.mkdir(BASE_PATH + GET_POST_PATH)
    os.mkdir(BASE_PATH + POST_DELETE_PATH)
    os.mkdir(BASE_PATH + ALL_METHODS_PATH)
    os.mkdir(BASE_PATH + NONE_ALLOW_METHODS_PATH)
    yield
    if os.path.exists(BASE_PATH):
        shutil.rmtree(BASE_PATH)


def touch_if_not_exist(path: str):
    if not os.path.isfile(path):
        with open(path, "w"):
            pass


def remove_if_exist(path: str):
    if os.path.isfile(path):
        os.remove(path)


def request_by_get(path: str, expect_status_code: int):
    url = BASE_URL + path
    full_path = BASE_PATH + path
    touch_if_not_exist(full_path)
    res = requests.get(url)
    remove_if_exist(full_path)
    assert res.status_code == expect_status_code


def request_by_post(path: str, expect_status_code: int):
    url = BASE_URL + path
    full_path = BASE_PATH + path
    remove_if_exist(full_path)
    res = requests.post(url)
    remove_if_exist(full_path)
    assert res.status_code == expect_status_code


def request_by_delete(path: str, expect_status_code: int):
    url = BASE_URL + path
    full_path = BASE_PATH + path
    touch_if_not_exist(full_path)
    res = requests.delete(url)
    remove_if_exist(full_path)
    assert res.status_code == expect_status_code
