#!/bin/bash

VENV_DIR="venv"
WEBSERV="webserv"
GRANDPARENT_DIR="../../"
WEBSERV_FROM_CURRENT_DIR=$GRANDPARENT_DIR$WEBSERV
WEBSERV_FROM_DETAIL_TEST_DIR=$GRANDPARENT_DIR$WEBSERV_FROM_CURRENT_DIR
export PYTHONDONTWRITEBYTECODE=1

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
		$WEBSERV_FROM_DETAIL_TEST_DIR *.conf 2>/dev/null &
		SERVER_PID=$!
		pytest -s
		EXIT_STATUS=$?
		cd -
		kill $SERVER_PID

		if [ $EXIT_STATUS -ne 0 ]
		then
			exit $EXIT_STATUS
		fi

	done
}

# call main function
main
