import os

envs = os.environ

assert envs.get("CONTENT_LENGTH") is None
assert envs.get("PATH_INFO") == "/123"
assert envs.get("PATH_TRANSLATED") == "./123"
assert envs.get("QUERY_STRING") == "p=1"
assert envs.get("SCRIPT_NAME") == "/cgi-bin/path_info_and_query.py"

print("Content-Type: text/html")
print("")
print("SUCCESS")
