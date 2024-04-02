#! /bin/bash

RESULT=0
EXEC_FILE="./webserv"

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
	test_samples=$2
	expected_return=$3

	echo "[ $test_title ]" ; echo
	for file in $test_samples
	do
		$EXEC_FILE $file && echo 
		if [ $? -eq $expected_return ] ; then
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
	test_samples=$2
	expected_return=$3

	echo "[ $test_title ]" ; echo
	for file in $test_samples
	do
		$EXEC_FILE $file > /dev/null 2>&1
		if [ $? -ne $expected_return ] ; then
			PRINT_COLOR $RED $file KO
			RESULT=1
		fi
	done
	echo
}

EXEC_REQUEST_PARSE() {
	http_sample_dir="./test/http_sample/"

	success_dir=$http_sample_dir'success/*/*'
	error_dir=$http_sample_dir'error/*/*'
	not_completed_dir=$http_sample_dir'request_not_completed/*'

	$1 'SUCCESS REQUEST TEST' "$success_dir" 0
	$1 'ERROR REQUEST TEST' "$error_dir" 1
	$1 'NOT COMPLETED REQUEST TEST' "$not_completed_dir" 2
}

if [ $1 == 'KO' ] ; then
	FUNC=_ONLY_KO
else
	FUNC=_RESULT
fi

if [ $2 == 'request_parse' ] ; then
	EXEC_REQUEST_PARSE $FUNC
fi

exit $RESULT
