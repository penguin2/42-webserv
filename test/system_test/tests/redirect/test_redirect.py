from conftest import request_with_redirect


def test_redirect_success():
    request_with_redirect("/redirect_success_start",
                          [301, 302, 303, 307, 308, 200])
    request_with_redirect("/redirect_success1", [302, 303, 307, 308, 200])
    request_with_redirect("/redirect_success2", [303, 307, 308, 200])
    request_with_redirect("/redirect_success3", [307, 308, 200])
    request_with_redirect("/redirect_success4", [308, 200])


def test_redirect_error():
    request_with_redirect("/redirect_error_start",
                          [301, 302, 303, 307, 308, 404])
    request_with_redirect("/redirect_error1", [302, 303, 307, 308, 404])
    request_with_redirect("/redirect_error2", [303, 307, 308, 404])
    request_with_redirect("/redirect_error3", [307, 308, 404])
    request_with_redirect("/redirect_error4", [308, 404])
