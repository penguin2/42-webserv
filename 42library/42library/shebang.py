from typing import Optional
import shutil


def write_first_line(file_path: str, line: str):
    with open(file_path, "r") as f:
        text_list = f.readlines()
    with open(file_path, "w") as f:
        f.write(line + "\n")
        f.write("".join(text_list))


def remove_first_line(file_path: str):
    with open(file_path, "r") as f:
        text_list = f.readlines()
    with open(file_path, "w") as f:
        f.write("".join(text_list[1:]))


def make_shebang_from_environment(command: str) -> str:
    """
    shutil.which  によってCGIの実行ファイルパスを取得
    CGIの実行ファイルが環境に存在しない場合は例外をthrow
    CGIの実行ファイルパスをシェバンに変換してreturn
    """
    cgi_exec_file: Optional[str] = shutil.which(command)
    if cgi_exec_file is None:
        raise EnvironmentError("CGI EXEC FILE is not found on your system.")
    shebang: str = "#!" + cgi_exec_file
    return shebang


def write_shebang_to_files(files: list[str], command: str):
    shebang: str = make_shebang_from_environment(command)
    for file in files:
        write_first_line(file, shebang)


def remove_shebang_from_files(files: list[str]):
    for file in files:
        remove_first_line(file)


if __name__ == "__main__":
    """
    シェバンの値は環境依存なのでサンプルサイトを動かす前に実行する
    新たにCGIスクリプトを追加した場合は下記のリストに追加する
    """
    write_shebang_to_files(
        ["./index.py", "./login.py", "./logout.py", "./signup.py"], "python3"
    )
