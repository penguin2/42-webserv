from conftest import request_by_get


def test_request_to_no_match_location():
    """
    LocationConfigの特定に失敗するケース
    """
    request_by_get("/", 404)
    request_by_get("/no_exist_directory", 404)


def test_request_to_match_location():
    """
    LocationConfigの特定に成功するケース
    """
    request_by_get("/match_location/abc.html", 200, "ABC")


def test_request_to_match_location_with_internal_redirect():
    """
    内部リダイレクト後にLocationConfigの特定に成功するケース
    (内部リダイレクトしている時点でLocationConfigのどれかにMatchすることは確定します)
    """
    request_by_get("/match_location/", 200, "MATCH")
