import os
import sys

envs = os.environ

assert envs.get("CONTENT_LENGTH") == "4"
assert envs.get("CONTENT_TYPE") == "text/html"
assert envs.get("QUERY_STRING") == ""
assert envs.get("SCRIPT_NAME") == "/cgi-bin/check_body.py"

body = sys.stdin.read(int(envs["CONTENT_LENGTH"]))

assert body == "BODY"

print("Content-Type: text/html")
print("")
print("SUCCESS")
