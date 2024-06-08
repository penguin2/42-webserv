from conftest import request_by_get
from conftest import SERVER1, SERVER2, SERVER3, SERVER4, SERVER5
from conftest import SERVER7, SERVER8, SERVER10, SERVER11

URL_SERVER1 = "http://127.0.0.1:4242"
URL_SERVER2 = "http://127.0.0.1:1234"
URL_SERVER3 = "http://127.0.0.2:4242"
URL_SERVER4 = "http://127.0.0.2:1234"
URL_SERVER567 = "http://127.0.0.3:4242"
URL_SERVER8 = "http://localhost:8765"
URL_SERVER9 = "http://localhost:4242"
URL_SERVER10 = "http://localhost:8888"
URL_SERVER11 = "http://localhost:9999"


def test_request_to_server():
    """
    IPが同じでPORTが異なる(server1 & server2, server3 & server4)
    PORTが同じでIPが異なる(server1 & server3, server2 & server4)
    """
    request_by_get(URL_SERVER1, SERVER1)
    request_by_get(URL_SERVER2, SERVER2)
    request_by_get(URL_SERVER3, SERVER3)
    request_by_get(URL_SERVER4, SERVER4)


def test_same_ip_and_port_server():
    """
    同一のIP＆PORTのサーバを3つ設定(server5, server6, server7)
    server_nameが同一の2つのサーバ(server5, server6)
    server_nameが異なる1つのサーバ(server7)
    indexディレクティブは全て異なる
    """
    # Hostを指定しなかったら同一IP&PORTの一番上のserverブロックにMatch
    request_by_get(URL_SERVER567, SERVER5)
    # server_nameが同一のserverブロックの中で一番上のブロックにMatch
    request_by_get(URL_SERVER567, SERVER5, {"Host": "server_same_name"})
    # (not Host in server_names) -> 同一IP&PORTの一番上のserverブロックにMatch
    request_by_get(URL_SERVER567, SERVER5, {"Host": "NONO"})
    # (server7のserver_name == Host) -> server7ブロックにMatch
    request_by_get(URL_SERVER567, SERVER7, {"Host": "server_another_name"})


def test_host_is_localhost():
    """
    localhostでlisten
    """
    request_by_get(URL_SERVER8, SERVER8)
    # localhost == 127.0.0.1
    # "localhost:4242" == "127.0.0.1:4242"
    request_by_get(URL_SERVER9, SERVER1)


def test_ip_not_set():
    request_by_get(URL_SERVER10, SERVER10)


def test_ip_is_0000():
    request_by_get(URL_SERVER11, SERVER11)


def test_no_setting_server():
    request_by_get("http://127.1.1.1:8888", SERVER10)
    request_by_get("http://127.2.2.2:9999", SERVER11)
