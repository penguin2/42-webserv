from sys import stdin

print("Content-Type: text/html")
print("")
print("(BODY) OK")
print("stdin.readline():", stdin.readline())

raise Exception

print("stdin.readline():", stdin.readline())
