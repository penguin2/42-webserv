from conftest import get_response_code_wrapped
from conftest import get_test_scripts
from concurrent.futures import ThreadPoolExecutor

NO_BODY_METHODS = ["get", "delete"]
ALL_METHODS = ["get", "post", "delete"]

def _make_testsets(methods: list[str], files: list[str], is_need_body: bool) -> list[tuple[str, str, bool]]:
    return [(method, file, is_need_body) for method in methods for file in files]

def _assert(testsets: list[tuple[str, str, bool]], expected_status: int):
    with ThreadPoolExecutor(max_workers=len(testsets)) as executor:
        response_statuses = executor.map(get_response_code_wrapped, testsets)
        assert all(status == expected_status for status in response_statuses)
    # for testset in testsets:
    #     assert get_response_code_wrapped(testset) == expected_status

def test_cgi_good():
    expected_status = 200
    testsets = []
    testsets += _make_testsets(NO_BODY_METHODS, get_test_scripts(expected_status, "NO-BODY"), False)
    testsets += _make_testsets(ALL_METHODS, get_test_scripts(expected_status, "BODY"), True)
    _assert(testsets, expected_status)

def test_cgi_error():
    expected_status = 500
    testsets = []
    testsets += _make_testsets(NO_BODY_METHODS, get_test_scripts(expected_status, "NO-BODY"), False)
    testsets += _make_testsets(ALL_METHODS, get_test_scripts(expected_status, "BODY"), True)
    _assert(testsets, expected_status)

def test_cgi_timeout():
    expected_status = 504
    testsets = []
    testsets += _make_testsets(NO_BODY_METHODS, get_test_scripts(expected_status, "NO-BODY"), False)
    testsets += _make_testsets(ALL_METHODS, get_test_scripts(expected_status, "BODY"), True)
    _assert(testsets, expected_status)
