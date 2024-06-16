#!/bin/bash

echo "Content-type: text/html"
echo ""
echo "<html><body>"
echo "<h1>Hello, CGI!</h1>"

# クエリパラメータから名前を取得
QUERY_STRING=$QUERY_STRING
name=$(echo "$QUERY_STRING" | grep -oP 'name=\K[^&]+')

if [ ! -z "$name" ]; then
    echo "<p>Hello, $name!</p>"
fi

echo "</body></html>"
