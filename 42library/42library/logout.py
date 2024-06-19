#!/usr/local/bin/python3
from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.table_data import SESSIONS
from business_logic.users_utils import get_user_if_exist
from presentation.generate_response import generate_logout_page
import os


def main():
    user: Optional[tuple] = get_user_if_exist()
    if user:
        user_id = user[0]
        db = LibraryDatabase()
        db.delete(SESSIONS, {"user_id": str(user_id)})
    host = os.environ["HTTP_HOST"]
    generate_logout_page(host)


if __name__ == "__main__":
    main()
