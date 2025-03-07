from conftest import check_post_response_code


def test_upload():
    check_post_response_code("/upload_on/index.html", 201)
    check_post_response_code("/upload_off/index.html", 403)
    check_post_response_code("/upload_none/index.html", 403)


def test_upload_on_with_cgi_ext():
    """
    upload on;
    cgi_ext .py;

    1. CGI成功(200)
    2. upload成功(201)
    3. 2で作成されたファイルがあり、CGI失敗(403)
    """
    check_post_response_code("/upload_on_with_cgi_ext/sample.py", 200)
    check_post_response_code("/upload_on_with_cgi_ext/NONO.py", 201)
    check_post_response_code("/upload_on_with_cgi_ext/NONO.py", 403)


def test_upload_off_with_cgi_ext():
    """
    upload off;
    cgi_ext .py;

    1. CGI成功(200)
    2. uploadがoffなのでupload失敗(403)
    3. 2と同様
    """
    check_post_response_code("/upload_off_with_cgi_ext/sample.py", 200)
    check_post_response_code("/upload_off_with_cgi_ext/NONO.py", 403)
    check_post_response_code("/upload_off_with_cgi_ext/NONO.py", 403)
