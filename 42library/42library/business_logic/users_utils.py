from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.library_database import USERS

import cgi


def check_user(user_name: str, password: str) -> Optional[tuple]:
    db = LibraryDatabase()
    user = db.select(USERS, {"user_name": user_name, "password": password})
    if user:
        return user[0]
    return None


def create_user(user_name: str, password: str) -> int:
    db = LibraryDatabase()
    user_id = db.insert_user(user_name, password)
    return user_id


def is_exist_user(user_name: str) -> bool:
    db = LibraryDatabase()
    user = db.select(USERS, {"user_name": user_name})
    if user:
        return True
    return False


def fetch_users() -> list:
    db = LibraryDatabase()
    return db.select(USERS)


def get_user_from_session(session: tuple) -> tuple:
    db = LibraryDatabase()
    user_id = session[3]
    user = db.select(USERS, {"user_id": str(user_id)})[0]
    return user


def get_user_if_exist() -> Optional[tuple]:
    form = cgi.FieldStorage()
    user_name = form.getvalue("username")
    password = form.getvalue("password")
    user = check_user(user_name, password)
    return user


if __name__ == "__main__":
    admin = check_user("admin", "admin")
    print(admin)
    users = fetch_users()
    print(users)
