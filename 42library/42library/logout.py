#!/usr/local/bin/python3
from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.table_data import SESSIONS
from business_logic.users_utils import get_user_if_exist
from presentation.generate_redirect_page import generate_redirect_logout_page
import os


def main():
    """
    Formデータからusernameとpassowrdを取得
    if (DBに照合、ユーザーが存在):
        userが所有するSessionを失効させる
    CookieからSessionIDを削除
    index.htmlにリダイレクト
    """
    user: Optional[tuple] = get_user_if_exist()
    if user:
        db = LibraryDatabase()
        user_id = user[0]
        db.delete(SESSIONS, {"user_id": str(user_id)})
    host = os.environ["HTTP_HOST"]
    generate_redirect_logout_page(host)


if __name__ == "__main__":
    main()
