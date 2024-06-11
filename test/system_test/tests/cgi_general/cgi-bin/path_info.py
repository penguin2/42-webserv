import os

envs = os.environ

assert envs.get("CONTENT_LENGTH") is None
assert envs.get("PATH_INFO") == "/abc"
assert envs.get("PATH_TRANSLATED") == "./abc"
assert envs.get("QUERY_STRING") == ""
assert envs.get("SCRIPT_NAME") == "/cgi-bin/path_info.py"

print("Content-Type: text/html")
print("")
print("SUCCESS")
