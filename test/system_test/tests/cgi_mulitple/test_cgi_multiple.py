from conftest import get_response_code
from conftest import get_test_scripts

def _assert_get_post_delete(path: str, status_code: int):
    for method in ["get", "post", "delete"]:
        assert get_response_code(method, path) == status_code

def test_cgi_py():
    for file in get_test_scripts(200, "py"):
        _assert_get_post_delete(file, 200)

def test_cgi_php():
    for file in get_test_scripts(200, "php"):
        _assert_get_post_delete(file, 200)
