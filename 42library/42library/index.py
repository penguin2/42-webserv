#!/usr/local/bin/python3
from business_logic.sessions_utils import get_session_from_envs
from business_logic.sessions_utils import update_session
from business_logic.users_utils import get_user_from_session
from presentation.generate_response import generate_redirect_page
from presentation.generate_index_py_response import generate_index_py_response
import os


def main():
    session = get_session_from_envs(os.environ)
    if session:
        update_session(session, 30)
        user = get_user_from_session(session)
        generate_index_py_response(user, session[0], 30)
    else:
        generate_redirect_page("/no_session.html")


if __name__ == "__main__":
    main()
