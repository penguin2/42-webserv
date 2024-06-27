#!/usr/local/bin/python3
from business_logic.sessions_utils import get_session_from_envs
from business_logic.sessions_utils import update_session
from business_logic.users_utils import get_user_from_session
from business_logic.add_new_book_data import add_new_book_data
from presentation.generate_redirect_page import generate_redirect_page
import os


def main():
    session = get_session_from_envs(os.environ)
    if session:
        update_session(session, additional_session_sec=30)
        user = get_user_from_session(session)
        user_id: int = int(user[0])

        if add_new_book_data(user_id):
            print("status: 201 Created")
            print("Content-Type: text/pre")
            print("")
            print("SUCCESS")
        else:
            print("status: 400 Bad Request")
            print("Content-Type: text/pre")
            print("")
            print("ERROR")
    else:
        generate_redirect_page("/42library/html/no_session.html")


if __name__ == "__main__":
    main()
