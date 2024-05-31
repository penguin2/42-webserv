#!/bin/bash

COMMAND="$1"
EXPECTED_EXIT_STATUS="$2"

TIMEOUT_DURATION=1

if [ -z "$COMMAND" ] || [ -z "$EXPECTED_EXIT_STATUS" ]; then
  echo "Usage: $0 <command> <expected_exit_status>"
  exit 1
fi

($COMMAND 2>/dev/null)&
COMMAND_PID=$!

(sleep $TIMEOUT_DURATION && kill -TERM $COMMAND_PID 2>/dev/null) &
SLEEP_PID=$!

wait $COMMAND_PID 2>/dev/null
COMMAND_EXIT_STATUS=$?

kill -TERM $SLEEP_PID 2>/dev/null
wait $SLEEP_PID

if [ $EXPECTED_EXIT_STATUS -eq 0 ]; then
    if [ $COMMAND_EXIT_STATUS -ne 0 ] && [ $COMMAND_EXIT_STATUS -ne 143 ]; then
        echo "expected status 0"
        echo "actual status $COMMAND_EXIT_STATUS"
        echo -e "\033[31mFail test :$COMMAND\033[m"
        exit 1
    else
        echo -e "\033[34mPass test\033[m"
        exit 0
    fi
else
    if [ $COMMAND_EXIT_STATUS -ne 0 ] && [ $COMMAND_EXIT_STATUS -ne 143 ]; then
        echo -e "\033[34mPass test\033[m"
        exit 0
    else
        echo "expected status 1"
        echo "actual status $COMMAND_EXIT_STATUS"
        echo -e "\033[31mFail test :$COMMAND\033[m"
        exit 1
    fi
fi