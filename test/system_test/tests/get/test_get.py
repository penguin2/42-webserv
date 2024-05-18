import requests


def test_sample():
    res = requests.get("http://127.0.0.1:4242/")
    # assert res.history[0].status_code == 301
    assert res.status_code == 405
