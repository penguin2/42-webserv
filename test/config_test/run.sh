#!/bin/bash

errors=0
passes=0

EXEC_FILE="../../webserv"
CONFIG_PATH="../../config_sample"

test_cases=(
    "$EXEC_FILE $CONFIG_PATH/OK/sample.conf 0"
    "$EXEC_FILE $CONFIG_PATH/OK/sample2.conf 0"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG1.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG2.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG3.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG4.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG5.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG6.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG7.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG8.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG9.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG10.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG11.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG12.conf 1"
    "$EXEC_FILE $CONFIG_PATH/NG/block/NG13.conf 1"
)

for test_case in "${test_cases[@]}"; do
    IFS=' ' read -r command  config_path expected_exit_status <<< "$test_case"
    
    echo "Running test: $command $config_path"
    ./parser_test.sh "$command $config_path" "$expected_exit_status"
    status=$?
    if [ $status -eq 0 ]; then
        ((passes++))
    else
        ((errors++))
    fi
done

echo "Test Summary:"
echo -e "\033[34mPasses: $passes\033[m"
echo -e "\033[31mFailures: $errors\033[m"

exit $errors
