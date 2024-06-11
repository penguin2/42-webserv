import os

envs = os.environ

assert envs.get("CONTENT_LENGTH") is None
assert envs.get("QUERY_STRING") == "space=%20"
assert envs.get("SCRIPT_NAME") == "/cgi-bin/query_string2.py"

print("Content-Type: text/html")
print("")
print("SUCCESS")
