#!/usr/local/bin/python3
from typing import Optional
from business_logic.users_utils import get_user_if_exist
from business_logic.sessions_utils import create_session
from presentation.generate_redirect_page import generate_redirect_page
from presentation.generate_redirect_page import generate_redirect_to_index_py
import os


def main():
    """
    Formデータからusernameとpassowrdを取得
    if (DBに照合、ユーザーが存在):
        Sessionを作成
        index.pyにSet-CookieでSessionIDを付与してリダイレクト
    else:
        login.htmlにリダイレクト
    """
    user: Optional[tuple] = get_user_if_exist()
    if user:
        user_id = user[0]
        session_id = create_session(user_id)
        host = os.environ["HTTP_HOST"]
        generate_redirect_to_index_py(session_id, host)
    else:
        generate_redirect_page("/login.html")


if __name__ == "__main__":
    main()
