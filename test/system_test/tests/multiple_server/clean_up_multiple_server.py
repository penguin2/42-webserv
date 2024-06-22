import os


def remove_file_if_exist(file: str):
    if os.path.isfile(file):
        os.remove(file)


if __name__ == "__main__":
    conf_file = "./multiple_server.conf"
    test_py_file = "./test_multiple_server.py"
    remove_file_if_exist(conf_file)
    remove_file_if_exist(test_py_file)
