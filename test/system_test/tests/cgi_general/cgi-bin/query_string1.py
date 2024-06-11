import os

envs = os.environ

assert envs.get("CONTENT_LENGTH") is None
assert envs.get("QUERY_STRING") == "q=50"
assert envs.get("SCRIPT_NAME") == "/cgi-bin/query_string1.py"

print("Content-Type: text/html")
print("")
print("SUCCESS")
