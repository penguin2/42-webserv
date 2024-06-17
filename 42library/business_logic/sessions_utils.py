from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.library_database import SESSIONS

import time
import os

SESSION_ID_KEY = "library_session_id"


def get_session_from_envs(envs: os._Environ) -> Optional[list]:
    if SESSION_ID_KEY not in envs:
        return None
    db = LibraryDatabase()
    session_list = db.select(SESSIONS, {"session_id": envs[SESSION_ID_KEY]})
    now = int(time.time())
    for session in session_list:
        # now < end_date && is_active == True
        if now < session[2] and session[3] == 1:
            return session
    return None


def update_session(session: list, new_session_date: int = 3600):
    db = LibraryDatabase()
    session_id = session[0]
    db.update(
        SESSIONS,
        {"end_date": str(new_session_date)},
        {"session_id": str(session_id)}
    )


def invalidate_session(session: list):
    db = LibraryDatabase()
    session_id = session[0]
    db.update(
        SESSIONS,
        {"is_active": "0"},
        {"session_id": session_id}
    )


def delete_session(session: list):
    session_id = session[0]
    db = LibraryDatabase()
    db.delete(SESSIONS, {"session_id": session_id})


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
