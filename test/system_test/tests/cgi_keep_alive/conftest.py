from system_test_utils.shebang import write_shebang_to_files
from system_test_utils.shebang import remove_shebang_from_files

import pytest
import glob

CGI_BIN_DIR = "./cgi-bin/"
PYTHON = "python3"


@pytest.fixture(scope="session", autouse=True)
def manage_shebang():
    cgi_scripts = glob.glob(CGI_BIN_DIR + "*.py")
    write_shebang_to_files(cgi_scripts, PYTHON)
    yield
    remove_shebang_from_files(cgi_scripts)
