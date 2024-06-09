from conftest import request_by_get, generate_random_dict

"""
requestsライブラリは自動でabsolute-formからorigin-formに変換するため
len("/") == 1Bytes
"""
MAX_URI_SIZE = 2000 - 1

KEY = "A"
"""
len(KEY) == 1Bytes
len(": ") == 2Bytes
合計3Bytes
"""
MAX_HEADER_SIZE = 200 - 3

"""
requestsライブラリが自動で下記の5つのヘッダを設定するため
1. User-Agent
2. Accept-Encoding
3. Accept
4. Connection
5. Host
"""
MAX_NUMBER_OF_HEADERS = 100 - 5


def test_limit_uri():
    request_by_get("NONO", 404)
    request_by_get('a' * (MAX_URI_SIZE), 404)
    request_by_get('a' * (MAX_URI_SIZE + 1), 414)


def test_limit_header_size():
    request_by_get("index.html", 200)
    request_by_get("index.html", 200, {KEY: 'a' * MAX_HEADER_SIZE})
    request_by_get("index.html", 431, {KEY: 'a' * (MAX_HEADER_SIZE + 1)})


def test_limit_number_of_headers():
    request_by_get("index.html", 200)
    request_by_get("index.html", 200,
                   generate_random_dict(MAX_NUMBER_OF_HEADERS))
    request_by_get("index.html", 431,
                   generate_random_dict(MAX_NUMBER_OF_HEADERS + 1))
