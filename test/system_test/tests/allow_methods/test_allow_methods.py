from conftest import request_by_get, request_by_post, request_by_delete
from conftest import GET_PATH, POST_PATH, DELETE_PATH
from conftest import GET_DELETE_PATH, GET_POST_PATH, POST_DELETE_PATH, ALL_METHODS_PATH, NONE_ALLOW_METHODS_PATH

TARGET_FILE = "index.html"


def test_request_to_allowd_get():
    path = GET_PATH + TARGET_FILE
    request_by_get(path, 200)
    request_by_post(path, 405)
    request_by_delete(path, 405)


def test_request_to_allowd_post():
    path = POST_PATH + TARGET_FILE
    request_by_get(path, 405)
    request_by_post(path, 201)
    request_by_delete(path, 405)


def test_request_to_allowd_delete():
    path = DELETE_PATH + TARGET_FILE
    request_by_get(path, 405)
    request_by_post(path, 405)
    request_by_delete(path, 204)


def test_request_to_allowd_get_and_delete():
    path = GET_DELETE_PATH + TARGET_FILE
    request_by_get(path, 200)
    request_by_post(path, 405)
    request_by_delete(path, 204)


def test_request_to_allowd_get_and_post():
    path = GET_POST_PATH + TARGET_FILE
    request_by_get(path, 200)
    request_by_post(path, 201)
    request_by_delete(path, 405)


def test_request_to_allowd_post_and_delete():
    path = POST_DELETE_PATH + TARGET_FILE
    request_by_get(path, 405)
    request_by_post(path, 201)
    request_by_delete(path, 204)


def test_request_to_allowd_all_methods():
    path = ALL_METHODS_PATH + TARGET_FILE
    request_by_get(path, 200)
    request_by_post(path, 201)
    request_by_delete(path, 204)


def test_request_to_none_allow_methods_directive():
    path = NONE_ALLOW_METHODS_PATH + TARGET_FILE
    request_by_get(path, 200)
    request_by_post(path, 201)
    request_by_delete(path, 204)
