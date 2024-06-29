#!/usr/local/bin/python3
from business_logic.users_utils import is_exist_user
from business_logic.users_utils import create_user
from business_logic.sessions_utils import create_session
from presentation.generate_redirect_page import generate_redirect_page
from presentation.generate_redirect_page import generate_redirect_to_index_py
from business_logic.sessions_utils import SESSION_SEC
import cgi
import os


def main():
    """
    Formデータからusernameとpassowrdを取得
    if (Formが適切な値 and usernameと同じ名前のユーザが存在しない):
        Userを作成
        Sessionを作成
        index.pyにSet-CookieでSessionIDを付与してリダイレクト
    else:
        signup.htmlにリダイレクト
    """
    form = cgi.FieldStorage()
    user_name = form.getvalue("username")
    password = form.getvalue("password")

    if user_name and password and not is_exist_user(user_name):
        user_id = create_user(user_name, password)
        session_id = create_session(user_id, SESSION_SEC)
        host = os.environ["HTTP_HOST"]
        generate_redirect_to_index_py(session_id, host)
    else:
        generate_redirect_page("/42library/html/signup.html")


if __name__ == "__main__":
    main()
