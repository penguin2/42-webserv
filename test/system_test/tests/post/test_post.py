from conftest import request_by_post, post_to_exist_dir
from conftest import post_limit_body_size, post_to_dir_without_permission


def test_simple_post():
    request_by_post("/test1", "abc", 201)
    request_by_post("/test2", "abc", 201)
    request_by_post("/././abc", "abc", 201)


def test_create_file_in_nonexistent_directory():
    """
    存在しないディレクトリにファイルを作成しようとする場合のテスト
    """
    request_by_post("/dir/test2", "nonexist dir", 500)


def test_post_to_exist_dir():
    post_to_exist_dir("/new_dir", 405)


def test_post_to_exist_file():
    """
    ファイルを作成
    同じファイルをPOSTした時のテスト
    """
    request_by_post("/new_file", "abc", 201)
    request_by_post("/new_file", "exist file", 405)


def test_no_permission_dir():
    post_to_dir_without_permission("/new_dir_no_permission", "/new_file", 500)


def test_client_max_body_size_error():
    post_limit_body_size("/index.html", "client max body limit", 413)
