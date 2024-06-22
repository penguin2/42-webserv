import socket


def get_ip_addr():
    host_name = socket.gethostname()
    ip_addr = socket.gethostbyname(host_name)
    return ip_addr


def make_multiple_server_conf(ip_addr: str, file: str):
    conf = f"""http {{
    server {{
        listen 127.0.0.1:4242;
        server_name server1;

        location / {{
            root ./html;
            index SERVER1;
        }}
    }}
    server {{
        listen 127.0.0.1:1234;
        server_name server2;

        location / {{
            root ./html;
            index SERVER2;
        }}
    }}
    server {{
        listen {ip_addr}:4242;
        server_name server3;

        location / {{
            root ./html;
            index SERVER3;
        }}
    }}
    server {{
        listen {ip_addr}:1234;
        server_name server4;

        location / {{
            root ./html;
            index SERVER4;
        }}
    }}

    server {{
        listen {ip_addr}:4567;
        server_name server_same_name;

        location / {{
            root ./html;
            index SERVER5;
        }}
    }}
    server {{
        listen {ip_addr}:4567;
        server_name server_same_name;

        location / {{
            root ./html;
            index SERVER6;
        }}
    }}
    server {{
        listen {ip_addr}:4567;
        server_name server_another_name;

        location / {{
            root ./html;
            index SERVER7;
        }}
    }}

    server {{
        listen localhost:8765;
        server_name localhost_is_unique;

        location / {{
            root ./html;
            index SERVER8;
        }}
    }}

    server {{
        listen localhost:4242;
        server_name localhost_is_same_server1;

        location / {{
            root ./html;
            index SERVER9;
        }}
    }}

    server {{
        listen 8888;
        server_name ip_is_none;

        location / {{
            root ./html;
            index SERVER10;
        }}
    }}

    server {{
        listen 0.0.0.0:9999;
        server_name ip_is_same_server1;

        location / {{
            root ./html;
            index SERVER11;
        }}
    }}
}}"""
    with open(file, "w") as f:
        f.write(conf)


def make_multiple_server_py(ip_addr: str, file: str):
    test_multiple_server = f"""
from conftest import request_by_get
from conftest import SERVER1, SERVER2, SERVER3, SERVER4, SERVER5
from conftest import SERVER7, SERVER8, SERVER10, SERVER11

URL_SERVER1 = "http://127.0.0.1:4242"
URL_SERVER2 = "http://127.0.0.1:1234"
URL_SERVER3 = "http://{ip_addr}:4242"
URL_SERVER4 = "http://{ip_addr}:1234"
URL_SERVER567 = "http://{ip_addr}:4567"
URL_SERVER8 = "http://localhost:8765"
URL_SERVER9 = "http://localhost:4242"
URL_SERVER10 = "http://localhost:8888"
URL_SERVER11 = "http://localhost:9999"


def test_request_to_server():
    request_by_get(URL_SERVER1, SERVER1)
    request_by_get(URL_SERVER2, SERVER2)
    request_by_get(URL_SERVER3, SERVER3)
    request_by_get(URL_SERVER4, SERVER4)


def test_same_ip_and_port_server():
    # Hostを指定しなかったら同一IP&PORTの一番上のserverブロックにMatch
    request_by_get(URL_SERVER567, SERVER5)
    # server_nameが同一のserverブロックの中で一番上のブロックにMatch
    request_by_get(URL_SERVER567, SERVER5, {{"Host": "server_same_name"}})
    # (not Host in server_names) -> 同一IP&PORTの一番上のserverブロックにMatch
    request_by_get(URL_SERVER567, SERVER5, {{"Host": "NONO"}})
    # (server7のserver_name == Host) -> server7ブロックにMatch
    request_by_get(URL_SERVER567, SERVER7, {{"Host": "server_another_name"}})


def test_host_is_localhost():
    request_by_get(URL_SERVER8, SERVER8)
    # localhost == 127.0.0.1
    # "localhost:4242" == "127.0.0.1:4242"
    request_by_get(URL_SERVER9, SERVER1)


def test_ip_not_set():
    request_by_get(URL_SERVER10, SERVER10)


def test_ip_is_0000():
    request_by_get(URL_SERVER11, SERVER11)
"""
    with open(file, "w") as f:
        f.write(test_multiple_server)


if __name__ == "__main__":
    ip_addr = get_ip_addr()
    conf_file = "./multiple_server.conf"
    test_py_file = "./test_multiple_server.py"
    make_multiple_server_conf(ip_addr, conf_file)
    make_multiple_server_py(ip_addr, test_py_file)
