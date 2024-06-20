#!/bin/bash

VENV_DIR="venv"
WEBSERV="webserv"
GRANDPARENT_DIR="../../"
WEBSERV_FROM_CURRENT_DIR=$GRANDPARENT_DIR$WEBSERV
WEBSERV_FROM_DETAIL_TEST_DIR=$GRANDPARENT_DIR$WEBSERV_FROM_CURRENT_DIR
MULTIPLE_SERVER_DIR="./tests/multiple_server"
export PYTHONDONTWRITEBYTECODE=1
PWD=$(pwd)
export PYTHONPATH="$PWD:$PYTHONPATH"

function main () {
    GREP_STR=$1
	init_venv_environment
	prep_multiple_server_test
	exec_webserv $GREP_STR
	clean_up_multiple_server_test
	clean_up
}

function init_venv_environment () {
	python3 -m venv $VENV_DIR
	source $VENV_DIR/bin/activate
	pip install --upgrade pip
	pip install -r requirements.txt
}

function prep_multiple_server_test () {
	PREP_MULTIPLE_SERVER="prep_multiple_server.py"
	cd $MULTIPLE_SERVER_DIR
	python3 $PREP_MULTIPLE_SERVER
	cd $GRANDPARENT_DIR
}

function clean_up_multiple_server_test () {
	CLEAN_UP_MULTIPLE_SERVER="clean_up_multiple_server.py"
	cd $MULTIPLE_SERVER_DIR
	python3 $CLEAN_UP_MULTIPLE_SERVER
	cd $GRANDPARENT_DIR
}

function clean_up () {
	deactivate
}

function exec_webserv () {
	for dir in tests/*$1*
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
main $1
