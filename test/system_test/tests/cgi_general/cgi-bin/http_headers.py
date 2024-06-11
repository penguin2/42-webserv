import os

envs = os.environ

assert envs.get("HTTP_MY_HEADER1") == "ABC"
assert envs.get("HTTP_MY_HEADER2") == "DEF"
assert envs.get("HTTP_HTTP_MY_HEADER3") == ""

print("Content-Type: text/html")
print("")
print("SUCCESS")
