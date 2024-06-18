from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.library_database import SESSIONS

import http.cookies
import time
import os

SESSION_ID_KEY = "library_session_id"


def get_session_from_envs(envs: os._Environ) -> Optional[list]:
    if "HTTP_COOKIE" not in envs:
        return None
    cookie = http.cookies.SimpleCookie(envs["HTTP_COOKIE"])
    session_id = cookie.get(SESSION_ID_KEY)
    if session_id is None:
        return None
    db = LibraryDatabase()
    session_list = db.select(SESSIONS, {"session_id": session_id.value})
    now = int(time.time())
    for session in session_list:
        if now < session[2]:
            return session
    return None


def update_session(session: list, new_session_date: int = 3600):
    db = LibraryDatabase()
    session_id = session[0]
    db.update(
        SESSIONS,
        {"end_date": str(int(time.time()) + new_session_date)},
        {"session_id": str(session_id)}
    )


def delete_session(session: list):
    session_id = session[0]
    db = LibraryDatabase()
    db.delete(SESSIONS, {"session_id": session_id})


def create_session(user_id: int) -> str:
    db = LibraryDatabase()
    session: tuple = db.insert_session(user_id)
    session_id: str = session[0]
    return session_id


if __name__ == "__main__":
    envs = {SESSION_ID_KEY: "1"}
    sess = get_session_from_envs(envs)
    assert sess is not []
    if sess:
        update_session(sess, int(time.time()) + 3600)
    sess = get_session_from_envs(envs)
    assert sess is not []
    if sess:
        delete_session(sess)
    sess = get_session_from_envs(envs)
    assert sess is None
