from typing import Optional
from persistence.database_manager import DatabaseManager
from persistence.table_data import USERS_TABLE, USERS_FOREIGN_KEYS
from persistence.table_data import BOOKS_TABLE, BOOKS_FOREIGN_KEYS
from persistence.table_data import SESSIONS_TABLE, SESSIONS_FOREIGN_KEYS
from persistence.table_data import BOOK_LOANS_TABLE, BOOK_LOANS_FOREIGN_KEYS
from persistence.table_data import USERS, BOOKS, SESSIONS, BOOK_LOANS

import os
import time


def _random_str(size) -> str:
    tbl = bytes.maketrans(bytearray(range(256)),
                          bytearray([ord(b'a') + b % 26 for b in range(256)]))
    # generate random bytes and translate them to lowercase ascii
    return os.urandom(size).translate(tbl).decode()


class LibraryDatabase:
    def __init__(self):
        self.__db_name = "42library.db"
        self.db = self.__init_library()

    def __init_library(self):
        is_exist_db = os.path.exists(self.__db_name)
        db = DatabaseManager(self.__db_name)
        if not is_exist_db:
            self.__create_library()
            self.__insert_dummy_data()
        return db

    def __create_library(self):
        self.db = DatabaseManager(self.__db_name)
        self.db.create_table(USERS, USERS_TABLE, USERS_FOREIGN_KEYS)
        self.db.create_table(BOOKS, BOOKS_TABLE, BOOKS_FOREIGN_KEYS)
        self.db.create_table(SESSIONS, SESSIONS_TABLE, SESSIONS_FOREIGN_KEYS)
        self.db.create_table(BOOK_LOANS, BOOK_LOANS_TABLE,
                             BOOK_LOANS_FOREIGN_KEYS)

    def __insert_dummy_data(self):
        self.insert_user("admin", "admin")
        self.insert_user("rikeda", "1234")
        self.insert_user("noname", "nono")
        self.insert_book("school", "100", 1)
        self.insert_book("food", "200", 1)

    def insert_user(self, name: str, password: str) -> int:
        columns = {"user_name": name, "password": password}
        cursor = self.db.insert(USERS, columns)
        return cursor.lastrowid

    def insert_book(self,
                    title: str,
                    isbn: str,
                    owner_id: int,
                    max_loan_sec=0) -> int:
        columns = {"title": title, "isbn": isbn, "owner_user_id": owner_id}
        if max_loan_sec:
            columns["max_loan_sec"] = max_loan_sec
        cursor = self.db.insert(BOOKS, columns)
        return cursor.lastrowid

    def insert_session(self, user_id: int, session_duration_sec=None) -> tuple:
        self.db.delete(SESSIONS, {"user_id": str(user_id)})
        columns = {"session_id": _random_str(10), "user_id": user_id}
        if session_duration_sec:
            session_end_date = int(time.time()) + session_duration_sec
            columns["end_date"] = session_end_date
        self.db.insert(SESSIONS, columns)
        cursor = self.db.select(SESSIONS, {"user_id": str(user_id)})
        return cursor.fetchone()

    def insert_book_loan(self,
                         book_id: int,
                         user_id: int,
                         max_loan_sec: int = 0) -> int:
        columns = {"book_id": book_id, "loan_user_id": str(user_id)}
        if max_loan_sec:
            columns["checkout_date"] = str(int(time.time()) + max_loan_sec)
        cursor = self.db.insert(BOOK_LOANS, columns)
        return cursor.lastrowid

    def select(self, table: str,
               conditions: dict[str, str] = {},
               order_by: dict[str, str] = {}) -> list:
        cursor = self.db.select(table, conditions, order_by)
        return cursor.fetchall()

    def update(self, table: str,
               data_dict: dict[str, str],
               conditions: dict[str, str]) -> Optional[tuple]:
        cursor = self.db.update(table, data_dict, conditions)
        return cursor.fetchone()

    def delete(self, table: str, conditions: dict[str, str]):
        self.db.delete(table, conditions)


if __name__ == "__main__":
    db = LibraryDatabase()
    new_session = db.insert_session(1, 3600)
    print(new_session)
