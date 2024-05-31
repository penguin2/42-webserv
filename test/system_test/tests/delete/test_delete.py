from conftest import request_by_delete, touch_and_delete, mkdir_and_delete
from conftest import delete_file_without_permission, delete_file_in_dir_without_permission


def test_simple_delete():
    touch_and_delete("/new_file", 204)
    touch_and_delete("/././abc", 204)


def test_file_not_exist():
    request_by_delete("/no_exist_file", 403)


def test_delete_dir():
    mkdir_and_delete("/directory", 403)


def test_no_permission_file():
    delete_file_without_permission("/no_permission", 403)


def test_no_permission_dir():
    delete_file_in_dir_without_permission("/dir", "/file", 403)
