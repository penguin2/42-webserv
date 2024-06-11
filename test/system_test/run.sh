#!/bin/bash

VENV_DIR="venv"
WEBSERV="webserv"
GRANDPARENT_DIR="../../"
WEBSERV_FROM_CURRENT_DIR=$GRANDPARENT_DIR$WEBSERV
WEBSERV_FROM_DETAIL_TEST_DIR=$GRANDPARENT_DIR$WEBSERV_FROM_CURRENT_DIR
export PYTHONDONTWRITEBYTECODE=1
PWD=$(pwd)
export PYTHONPATH="$PWD:$PYTHONPATH"

function main () {
	init_venv_environment
	exec_webserv
	clean_up
}

function init_venv_environment () {
	python3 -m venv $VENV_DIR
	source $VENV_DIR/bin/activate
	pip install --upgrade pip
	pip install -r requirements.txt
}

function clean_up () {
	deactivate
}

function exec_webserv () {
	for dir in tests/*
	do
		cd $dir
		$WEBSERV_FROM_DETAIL_TEST_DIR *.conf &
		SERVER_PID=$!
		pytest -s
		cd -
		kill $SERVER_PID
	done
}

# call main function
main
