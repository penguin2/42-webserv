#!/bin/bash

WEBSERV="../../webserv"

for dir in tests/*
do
	$WEBSERV $dir/*.conf &
	SERVER_PID=$!
	./k6/k6 run $dir/test.js
	kill $SERVER_PID
done
