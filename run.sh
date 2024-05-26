#!/bin/bash

errors=0
passes=0

test_cases=(
    "./webserv ./config_sample/OK/sample.conf 0"
    "./webserv ./config_sample/OK/sample2.conf 0"
    "./webserv ./config_sample/NG/sample2.conf 0"
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
