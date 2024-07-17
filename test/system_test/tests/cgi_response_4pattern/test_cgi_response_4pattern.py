from conftest import request_to_cgi
import requests


def _check_request_to_cgi(path: str, expect_status_code: int):
    methods = ["get", "post", "delete"]
    responses: list[requests.Response] = [
        request_to_cgi(method, path) for method in methods
    ]

    for response in responses:
        assert response.status_code == expect_status_code


def test_document_response():
    _check_request_to_cgi("document_response-200.py", 200)
    _check_request_to_cgi("document_response_have_status_header-456.py", 456)


def test_local_redirect_response():
    _check_request_to_cgi("local_redirect_response-200.py", 200)


# LocalRedirectでLocationに指定されたリソースがない場合
# 静的ファイルの処理でのエラーステータスコードが返る
def test_local_redirect_response_no_file():
    path = "local_redirect_response_no_file.py"
    assert request_to_cgi("get", path).status_code == 404
    assert request_to_cgi("post", path).status_code == 403
    assert request_to_cgi("delete", path).status_code == 404


# CGIスクリプト内部で自分自身にリダイレクトし、ローカルリダイレクトの無限ループが起こる
# ループだが504(Gateway Timeout)ではなくLocalRedirect上限に達したことによる500が返る
def test_local_redirect_response_loop():
    _check_request_to_cgi("local_redirect_response_loop-500.py", 500)


def test_client_redirect_response():
    _check_request_to_cgi("client_redirect_response-302.py", 302)


def test_client_redirect_response_with_document():
    _check_request_to_cgi("client_redirect_response_with_document-301.py", 301)
    _check_request_to_cgi("client_redirect_response_with_document-302.py", 302)
    _check_request_to_cgi("client_redirect_response_with_document-303.py", 303)
    _check_request_to_cgi("client_redirect_response_with_document-307.py", 307)
    _check_request_to_cgi("client_redirect_response_with_document-308.py", 308)
