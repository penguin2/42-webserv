MAX_RETRY = 10


def retry_on_exception(my_func):
    """
    例外が発生してもMAX_RETRY回リトライ
    """
    def _wrapper(*args, **kwargs):
        retry_count = 0
        while True:
            try:
                my_func(*args, **kwargs)
                break
            except Exception as e:
                if MAX_RETRY <= retry_count:
                    print("Maximum number of retries reached")
                    raise e
                retry_count += 1
                print("RETRY[", retry_count, "]")
    return _wrapper
