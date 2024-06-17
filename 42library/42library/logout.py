#!/bin/python3
from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.table_data import SESSIONS
from business_logic.users_utils import check_user
from presentation.generate_response import generate_logout_page
import cgi
import os


def get_user_if_exist() -> Optional[tuple]:
    form = cgi.FieldStorage()
    user_name = form.getvalue('username')
    password = form.getvalue('password')
    user = check_user(user_name, password)
    return user


def create_session(user_id: int) -> str:
    db = LibraryDatabase()
    session: tuple = db.insert_session(user_id)
    session_id: str = session[0]
    return session_id


def main():
    user: Optional[tuple] = get_user_if_exist()
    if user:
        user_id = user[0]
        db = LibraryDatabase()
        db.delete(SESSIONS, {"user_id": str(user_id)})
    host = os.environ["HTTP_HOST"]
    generate_logout_page(host)


if __name__ == "__main__":
    main()
