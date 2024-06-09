from conftest import request_by_thread_pool
from colorama import init, Fore, Style

# WebservのデフォルトのHTTPリクエストのタイムアウト値
BORDER_TIME = 5
TWICE_BORDER_TIME = BORDER_TIME * 2


def print_warn_timeout(max_sleep_time: float):
    """
    Timeoutテストの実行に何秒かかるかを色付きで表示
    """
    init()  # coloramaを初期化
    print("")
    print(Fore.MAGENTA)
    print("TimeoutTest may take up to ", end='')
    print(Fore.BLUE, end='')
    print(max_sleep_time, end='')
    print(Fore.MAGENTA, end='')
    print(" seconds")
    print(Style.RESET_ALL)


def test_timeout():
    """
    times_and_codes -> List[Tuple[遅延させる秒数, ステータスコード or None]]
    """
    times_and_codes = [
        (0, 200),
        (BORDER_TIME / 2, 200),
        (BORDER_TIME - 1, 200),
        (BORDER_TIME + 1, 408),
        (TWICE_BORDER_TIME - 1, 408),
        (TWICE_BORDER_TIME + 1, 408)
    ]
    sleep_times = [sleep_time for sleep_time, _ in times_and_codes]
    expect_codes = [expect_code for _, expect_code in times_and_codes]
    print_warn_timeout(max(sleep_times))
    request_by_thread_pool(sleep_times, expect_codes)
