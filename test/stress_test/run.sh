#!/bin/bash

WEBSERV="../../webserv"
UPLOAD_DIR="../../html/upload"

for dir in tests/*
do
	$WEBSERV $dir/*.conf &
	SERVER_PID=$!
	./k6/k6 run $dir/test.js
	kill $SERVER_PID
	rm $UPLOAD_DIR/*.json
done
