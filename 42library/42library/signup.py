#!/bin/python3
from business_logic.users_utils import is_exist_user
from business_logic.users_utils import create_user
from business_logic.sessions_utils import create_session
from presentation.generate_response import generate_redirect_page
from presentation.generate_response import generate_redirect_to_index_py
import cgi
import os


def main():
    form = cgi.FieldStorage()
    user_name = form.getvalue('username')
    password = form.getvalue('password')

    if not is_exist_user(user_name):
        user_id = create_user(user_name, password)
        session_id = create_session(user_id)
        host = os.environ["HTTP_HOST"]
        generate_redirect_to_index_py(session_id, host)
    else:
        generate_redirect_page("/signup.html")


if __name__ == "__main__":
    main()
