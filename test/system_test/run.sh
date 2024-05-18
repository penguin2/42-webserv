#!/bin/bash

VENV_DIR=venv
WEBSERV="../../webserv"

python3 -m venv $VENV_DIR
source $VENV_DIR/bin/activate
pip install --upgrade pip
pip install -r requirements.txt


for dir in tests/*
do
	$WEBSERV $dir/*.conf &
	SERVER_PID=$!
	pytest $dir/
	kill $SERVER_PID
done

# CLEAN UP
deactivate
