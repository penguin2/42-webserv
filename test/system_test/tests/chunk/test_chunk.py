from conftest import check_chunk_upload
from conftest import check_chunk_cgi
from conftest import PYTHON_SCRIPT

import random
import string


def _create_random_string(size: int) -> str:
    charset = string.ascii_letters + string.digits
    return "".join([random.choice(charset) for _ in range(size)])


def test_chunked_upload():
    check_chunk_upload("post1", _create_random_string(1))
    check_chunk_upload("post2", _create_random_string(10))
    check_chunk_upload("post3", _create_random_string(100))
    check_chunk_upload("post4", _create_random_string(1000))


def test_chunked_cgi():
    check_chunk_cgi(PYTHON_SCRIPT, _create_random_string(1))
    check_chunk_cgi(PYTHON_SCRIPT, _create_random_string(10))
    check_chunk_cgi(PYTHON_SCRIPT, _create_random_string(100))
    check_chunk_cgi(PYTHON_SCRIPT, _create_random_string(1000))
