def f():
    return 1 + f()

print("Content-Type: text/html")
print("")
print("(BODY) infinite recursion")
print("f():", f())
