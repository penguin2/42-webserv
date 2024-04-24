### test

# ブロックについて
* 1.コンテキストの } が足りない
* 2.スペースのみの行が含まれる
* 3.コンテキストの順序がおかしい(http -> location)
* 4.存在しないcontextがある
* 5.httpが複数存在する
* 6.locationのあとにpathが存在しない
* 7.locationのあとに { が足りない
* 8.locationのあとのpathに使用できない文字が含まれている
* 9.httpのなかにserverが存在しない
* 10.serverの中にlocationが存在しない
* 11.httpの後に余計な引数が存在する
* 12.serverの後に余計な引数が存在する
* 13.}の後に余計な文字が存在する
...

# ディレクティブについて
* 1. 要素に対してContextがあっていない
   * 1. 正常なコンテキストに存在しない要素が含まれている場合
   * 2. ブロック要素が含まれるべき箇所に単一のディレクティブが存在する場合
* 2. 存在しない要素
   * 1. 不明なディレクティブが含まれている場合
* 3. 行末にセミコロンが存在しない
   * 1. ディレクティブの最後にセミコロンがない場合
* 4. listen
   * 1. listen 引数にportが存在しない
   * 2. listen 引数にIPアドレスが存在しない
   * 3. listen 引数が数字ではない
   * 4. listen 引数が複数存在する
   * 5. listen 引数のportが範囲外(0～65535)
* 5. server_name
   * 1. server_nameに引数がない
   * 2. server_nameに複数の引数がある
   * 3. server_nameに使用不可能な文字が含まれる場合
* 6. error_page
   * 1. error_pageにPathの引数が存在しない
   * 2. error_pageに複数のPathの引数がある
* 7. allow_methods
   * 1. allow_methodsにつづくメソッドが存在しない
* 8. root
   * 1. rootに引数がない
   * 2. rootに複数の引数がある
* 9. index
   * 1. indexに引数がない
   * 2. indexに複数の引数がある
* 10. autoindex
   * 1. autoindexに引数がない
   * 2. autoindexに複数の引数がある
* 11. return
   * 1. 301, 302, 303, 307, 308以外の引数がある
...
