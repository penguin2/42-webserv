#!/bin/bash
echo "content-type: text/plain"
echo ""

for file in *
do
    echo ""
    echo "[" ${file} "]"
    echo ""
    echo "----------------------------------------"
    cat ${file}
    echo "----------------------------------------"
done
