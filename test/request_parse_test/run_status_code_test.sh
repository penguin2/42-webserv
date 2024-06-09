#!/bin/bash

RESULT=0
EXEC_FILE="../../webserv"

ESC='\e['
RED=31
GREEN=32
YELLOW=33
BLUE=34


PRINT_COLOR() {
	if [ $1 -eq $RED ] ; then
		printf "${ESC}${RED}m %s ... %s ${ESC}m\n" $2 $3
	elif [ $1 -eq $GREEN ] ; then
		printf "${ESC}${GREEN}m %s ... %s ${ESC}m\n" $2 $3
	elif [ $1 -eq $BLUE ] ; then
		printf "${ESC}${BLUE}m %s ... %s ${ESC}m\n" $2 $3
	elif [ $1 -eq $YELLOW ] ; then
		printf "${ESC}${YELLOW}m %s ... %s ${ESC}m\n" $2 $3
	else
		printf "%s ... %s\n" $2 $3
	fi
}

_RESULT() { 
	test_title=$1
	test_dir=$2
	expected_error_code=$3

	echo "[ $test_title ]" ; echo
	for file in "$test_dir"/*
	do
		$EXEC_FILE $file $expected_error_code && echo 
		if [ $? -eq 1 ] ; then
			PRINT_COLOR $GREEN $file OK
		else
			PRINT_COLOR $RED $file KO
			RESULT=1
		fi
	done
	echo
}

_ONLY_KO() {
	test_title=$1
	test_dir=$2
	expected_error_code=$3

	echo "[ $test_title ]" ; echo
	for file in "$test_dir"/*
	do
		$EXEC_FILE $file $expected_error_code > /dev/null 2>&1
		if [ $? -ne 1 ] ; then
			PRINT_COLOR $RED $file KO
			RESULT=1
		fi
	done
	echo
}

EXEC_REQUEST_PARSE() {
	http_sample_dir="./http_sample/"

	error_dir=$http_sample_dir'error/*'

	for dir in $error_dir
	do
		expect_error_code=$(echo "$dir" | awk -F'error' '{print $3}')
		$1 "ERROR CODE $expect_error_code TEST" "$dir" "$expect_error_code"
	done

}

if [ $1 == 'KO' ] ; then
	FUNC=_ONLY_KO
else
	FUNC=_RESULT
fi

EXEC_REQUEST_PARSE $FUNC

exit $RESULT
