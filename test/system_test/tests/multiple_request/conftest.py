import pytest
import os
import shutil

TIMEOUT_SEC = 0.1

HTML_DIR = "./html/"


@pytest.fixture(scope="session", autouse=True)
def manage_html_directory():
    os.mkdir(HTML_DIR)
    with open(HTML_DIR + "/index.html", "w") as f:
        f.write("INDEX")
    yield
    if os.path.exists(HTML_DIR):
        shutil.rmtree(HTML_DIR)
