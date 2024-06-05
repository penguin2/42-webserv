from conftest import request_with_redirect


def test_redirect_success():
    """
    リダイレクトし、最終的なリダイレクト先にリソースが存在する場合
    """
    request_with_redirect("/redirect_success_start",
                          [301, 302, 303, 307, 308, 200])
    request_with_redirect("/redirect_success1", [302, 303, 307, 308, 200])
    request_with_redirect("/redirect_success2", [303, 307, 308, 200])
    request_with_redirect("/redirect_success3", [307, 308, 200])
    request_with_redirect("/redirect_success4", [308, 200])


def test_redirect_error():
    """
    リダイレクトし、最終的なリダイレクト先にリソースがない場合
    """
    request_with_redirect("/redirect_error_start",
                          [301, 302, 303, 307, 308, 404])
    request_with_redirect("/redirect_error1", [302, 303, 307, 308, 404])
    request_with_redirect("/redirect_error2", [303, 307, 308, 404])
    request_with_redirect("/redirect_error3", [307, 308, 404])
    request_with_redirect("/redirect_error4", [308, 404])


def test_redirect_with_internal_redirect():
    """
    内部リダイレクトした後にリダイレクト
    成功パターン & 失敗パターン
    """
    request_with_redirect("/internal_redirect_success",
                          [301, 301, 302, 303, 307, 308, 200])
    request_with_redirect("/internal_redirect_error",
                          [301, 301, 302, 303, 307, 308, 404])
