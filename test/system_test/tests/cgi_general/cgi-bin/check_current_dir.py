import os
import pathlib

envs = os.environ

assert envs.get("SCRIPT_NAME") == "/cgi-bin/check_current_dir.py"

current_dir: pathlib.Path = pathlib.Path(os.getcwd())

assert current_dir.name == "cgi-bin"


print("Content-Type: text/html")
print("")
print("SUCCESS")
