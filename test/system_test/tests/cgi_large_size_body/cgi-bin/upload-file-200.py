import sys
from string import ascii_uppercase, digits
from random import choice
from os import environ

def random_str_generator(size = 10):
    return ''.join(choice(ascii_uppercase + digits) for _ in range(size))

print("Content-Type: text/html")
print("")

data = sys.stdin.buffer.read()
filename = environ.get("HTTP_FILENAME", random_str_generator())

print("file upload: start")
with open("upload/" + filename, "wb") as new_file:
    new_file.write(data)
print("file upload: done")
