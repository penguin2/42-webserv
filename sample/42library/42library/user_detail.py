#!/usr/local/bin/python3
from business_logic.sessions_utils import get_session_from_envs
from business_logic.sessions_utils import update_session
from business_logic.users_utils import get_user_from_session
from presentation.generate_redirect_page import generate_redirect_page
from presentation.generate_user_detail_page import generate_user_detail_page
from business_logic.sessions_utils import SESSION_SEC
import os


def main():
    session = get_session_from_envs(os.environ)
    if session:
        update_session(session, additional_session_sec=SESSION_SEC)
        user = get_user_from_session(session)
        session_id = session[0]
        generate_user_detail_page(user, session_id, max_age=SESSION_SEC)
    else:
        generate_redirect_page("/42library/html/no_session.html")


if __name__ == "__main__":
    main()
