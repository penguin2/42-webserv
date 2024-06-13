from system_test_utils.shebang import make_shebang_from_environment

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


def create_cgi_script(directory: str, file: str, command: str, text: str):
    shebang = make_shebang_from_environment(command)
    cgi_script = directory + file
    with open(cgi_script, "w") as f:
        f.write(shebang + "\n")
        f.write(text)
    os.chmod(cgi_script, 0o755)


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    os.mkdir(CGI_BIN_DIR)
    create_cgi_script(CGI_BIN_DIR, "index.py", PYTHON, CGI_SCRIPT_TEXT)
    yield
    if os.path.exists(CGI_BIN_DIR):
        shutil.rmtree(CGI_BIN_DIR)
