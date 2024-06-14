from os import urandom
from conftest import get_response

# https://stackoverflow.com/questions/16308989/fastest-method-to-generate-big-random-string-with-lower-latin-letters
# make translation table from 0..255 to 97..122
def _random_str(size):
    tbl = bytes.maketrans(bytearray(range(256)),
                          bytearray([ord(b'a') + b % 26 for b in range(256)]))
    # generate random bytes and translate them to lowercase ascii
    return urandom(size).translate(tbl)

CGI_BIN_DIR = "cgi-bin/"
UPLOAD_DIR = CGI_BIN_DIR + "upload/"
UPLOAD_SCRIPT = "cgi-bin/upload-file-200.py"

# 'post' content to 'UPLOAD_SCRIPT', and then 'get' the uploaded file
def _check_large_body(filename, size):
    large_content = _random_str(size)

    status_code, _ = get_response("post", UPLOAD_SCRIPT, {"FILENAME": filename}, large_content)
    assert status_code == 200

    status_code, response_content = get_response("get", UPLOAD_DIR + filename, {}, "")
    assert status_code == 200
    assert response_content == large_content

def test_cgi_large_size_body():
    one_kb = 2**10
    one_mb = 2**20
    ft_mb = 42 * one_mb
    hundred_mb = 100 * one_mb
    _check_large_body("small_size.txt", one_kb)
    _check_large_body("medium_size.txt", ft_mb)
    _check_large_body("large_size.txt", hundred_mb)
