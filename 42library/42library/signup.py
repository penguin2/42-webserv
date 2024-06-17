#!/bin/python3
from persistence.library_database import LibraryDatabase
from business_logic.users_utils import check_user
from presentation.generate_response import generate_redirect_page
from presentation.generate_response import generate_redirect_to_index_py
import cgi
import os


def create_user(user_name: str, password: str) -> int:
    db = LibraryDatabase()
    user_id = db.insert_user(user_name, password)
    return user_id


def create_session(user_id: int) -> str:
    db = LibraryDatabase()
    session: tuple = db.insert_session(user_id)
    session_id: str = session[0]
    return session_id


def main():
    form = cgi.FieldStorage()
    user_name = form.getvalue('username')
    password = form.getvalue('password')
    user = check_user(user_name, password)
    if user is None:
        user_id = create_user(user_name, password)
        session_id = create_session(user_id)
        host = os.environ["HTTP_HOST"]
        generate_redirect_to_index_py(session_id, host)
    else:
        generate_redirect_page("/signup.html")


if __name__ == "__main__":
    main()
