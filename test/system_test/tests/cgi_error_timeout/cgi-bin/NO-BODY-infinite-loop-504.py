print("Content-Type: text/html")
print("")
print("(BODY) infinite loop may result in cgi-timeout")

count = 0
while True:
    count += 1
