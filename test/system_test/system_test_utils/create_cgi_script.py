from system_test_utils.shebang import make_shebang_from_environment

import os


def create_cgi_script(cgi_script: str, command: str, text: str):
    shebang = make_shebang_from_environment(command)
    with open(cgi_script, "w") as f:
        f.write(shebang + "\n")
        f.write(text)
    os.chmod(cgi_script, 0o755)
