import os

envs = os.environ


assert envs.get("AUTH_TYPE") is None
assert envs.get("CONTENT_LENGTH") is None
assert envs.get("CONTENT_TYPE") is None
assert envs.get("GATEWAY_INTERFACE") == "CGI/1.1"
assert envs.get("PATH_INFO") is None
assert envs.get("PATH_TRANSLATED") is None
assert envs.get("QUERY_STRING") == ""
assert envs.get("REMOTE_ADDR") is not None
assert envs.get("REMOTE_HOST") == envs.get("REMOTE_ADDR")
assert envs.get("REMOTE_IDENT") is None
assert envs.get("REMOTE_USER") is None
assert envs.get("REQUEST_METHOD") == "POST"
assert envs.get("SCRIPT_NAME") == "/cgi-bin/meta_var_post.py"
assert envs.get("SERVER_NAME") == "server1"
assert envs.get("SERVER_PORT") == "4242"
assert envs.get("SERVER_PROTOCOL") == "HTTP/1.1"
assert envs.get("SERVER_SOFTWARE") == "Webserv"

print("Content-Type: text/html")
print("")
print("SUCCESS")
