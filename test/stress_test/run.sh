#!/bin/bash

WEBSERV="../../webserv"
UPLOAD_DIR="./html/upload"

mkdir $UPLOAD_DIR

for dir in tests/*
do
	$WEBSERV $dir/*.conf &
	SERVER_PID=$!
	./k6/k6 run $dir/test.js
	kill $SERVER_PID
done

rm -r $UPLOAD_DIR
