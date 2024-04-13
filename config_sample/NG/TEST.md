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

## ディレクティブの中身
* 1.要素に対してContextがあっていない
* 2.存在しない要素
* 3.行末にセミコロンが存在しない
* 4.listen 引数にportが存在しない
* 5.listen 引数にIPアドレスが存在しない
* 6.server_nameに引数がない
* 7.error_pageに対してPathの引数が存在しない
* 8.allow_methodsにつづくメソッドが存在しない
...
