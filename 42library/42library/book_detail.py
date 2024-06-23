#!/usr/local/bin/python3
from business_logic.sessions_utils import get_session_from_envs
from business_logic.sessions_utils import update_session
from business_logic.users_utils import get_user_from_session
from presentation.generate_redirect_page import generate_redirect_page
from presentation.generate_book_detail_page import generate_book_detail_page
import os


def main():
    """
    SessionIDを環境変数から取得
    if (session): Sessionを更新、カスタムページを組み立てる
    else: Sessionが無い場合用のページへリダイレクトする
    """
    session = get_session_from_envs(os.environ)
    if session:
        update_session(session, additional_session_sec=30)
        user = get_user_from_session(session)
        session_id = session[0]
        generate_book_detail_page(user, session_id, max_age=30)
    else:
        generate_redirect_page("/no_session.html")


if __name__ == "__main__":
    main()
