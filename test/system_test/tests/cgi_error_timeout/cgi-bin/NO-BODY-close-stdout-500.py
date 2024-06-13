from os import close
from time import sleep

print("Content-Type: text/html")
print("")
print("(BODY) closing stdout with not exiting program may result in server error")

close(1)

sleep(3)
