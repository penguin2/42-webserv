from typing import Optional
import cgi


def get_page_number_from_qstring() -> int:
    """
    QUERY_STRINGから'p'というkeyを取得
    'p'が存在しない・不正な値の場合は1ページ目を表示させる
    """
    form = cgi.FieldStorage()
    page_number = form.getvalue("p")
    try:
        page_number = int(page_number)
    except Exception:
        page_number = 1
    page_number = 1 if (page_number <= 0) else page_number

    return page_number


def get_id_from_qstring(id_key: str) -> Optional[int]:
    form = cgi.FieldStorage()
    id_value = form.getvalue(id_key)
    try:
        id_value = int(id_value)
    except Exception:
        id_value = None

    return id_value
