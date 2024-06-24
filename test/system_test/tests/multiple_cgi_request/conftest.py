from system_test_utils.create_cgi_script import create_cgi_script

import pytest
import os
import shutil

TIMEOUT_SEC = 0.1

CGI_BIN_DIR = "./cgi-bin/"
PYTHON = "python3"

CGI_SCRIPT_TEXT = """
print("Content-Type: text/html")
print("")
print("IS CGI")
"""


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    os.mkdir(CGI_BIN_DIR)
    with open(CGI_BIN_DIR + "index.html", "w") as f:
        f.write("INDEX")
    create_cgi_script(CGI_BIN_DIR + "index.py", PYTHON, CGI_SCRIPT_TEXT)
    yield
    if os.path.exists(CGI_BIN_DIR):
        shutil.rmtree(CGI_BIN_DIR)
