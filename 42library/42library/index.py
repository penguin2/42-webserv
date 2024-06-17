#!/bin/python3
from business_logic.sessions_utils import get_session_from_envs
from business_logic.sessions_utils import update_session
from persistence.library_database import LibraryDatabase
from persistence.table_data import USERS
from presentation.generate_response import generate_no_session_page
from presentation.generate_index_py_response import generate_index_py_response
import os


def get_user_from_session(session: tuple) -> tuple:
    db = LibraryDatabase()
    user_id = session[3]
    user = db.select(USERS, {"user_id": str(user_id)})[0]
    return user


def main():
    session = get_session_from_envs(os.environ)
    if session:
        update_session(session)
        user = get_user_from_session(session)
        generate_index_py_response(user)
    else:
        generate_no_session_page()


if __name__ == "__main__":
    main()
