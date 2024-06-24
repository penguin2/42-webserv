#!/usr/local/bin/python3
from typing import Optional
from business_logic.sessions_utils import get_session_from_envs
from business_logic.sessions_utils import update_session
from business_logic.users_utils import get_user_from_session
from business_logic.manage_book_data import manage_book_data
from presentation.generate_redirect_page import generate_redirect_page
from presentation.generate_redirect_page import generate_redirect_book_detail_page
import os
import cgi


def main():
    session = get_session_from_envs(os.environ)
    if session:
        update_session(session, additional_session_sec=30)
        user: tuple = get_user_from_session(session)

        form = cgi.FieldStorage()
        book_id: Optional[str] = form.getvalue("book_id")
        control: Optional[str] = form.getvalue("control")
        manage_book_data(user, book_id, control)

        session_id = session[0]
        generate_redirect_book_detail_page(
            session_id, os.environ.get("HTTP_REFERER"), os.environ["HTTP_HOST"])
    else:
        generate_redirect_page("/no_session.html")


if __name__ == "__main__":
    main()
