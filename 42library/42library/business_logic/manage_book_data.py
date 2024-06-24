from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from persistence.table_data import BOOK_LOANS
from business_logic.books_utils import is_loan_suspended, is_on_loan
from business_logic.books_utils import is_available_for_loan
import time
import os


def manage_book_data(user: tuple, book_id: Optional[str], control: Optional[str]):
    handlers = {
        "stop": _handler_stop,
        "delete": _handler_delete,
        "loan": _handler_loan,
        "return": _handler_return
    }
    if book_id is None or control is None:
        return
    try:
        handlers[control](user, book_id)
    except Exception:
        pass


def _handler_stop(user: tuple, book_id: str):
    book = _get_book(book_id)
    if book is None:
        return
    user_id = int(user[0])
    book_owner_id = int(book[5])
    if user_id == book_owner_id and is_on_loan(int(book_id)):
        db = LibraryDatabase()
        db.update(BOOKS, {"status": '2'}, {"book_id": book_id})


def _handler_delete(user: tuple, book_id: str):
    book = _get_book(book_id)
    if book is None:
        return
    can_delete_book = not is_on_loan(
        int(book_id)) and not is_loan_suspended(int(book_id))
    user_id = int(user[0])
    book_owner_id = int(book[5])
    if user_id == book_owner_id and can_delete_book:
        db = LibraryDatabase()
        db.delete(BOOKS, {"book_id": book_id})
        os.chmod(f"./../images/{book_id}.png", 0o755)


def _handler_loan(user: tuple, book_id: str):
    book = _get_book(book_id)
    if book is None:
        return
    if is_available_for_loan(int(book_id)):
        db = LibraryDatabase()
        user_id = int(user[0])
        max_loan_sec = int(book[3])
        db.insert_book_loan(int(book_id), user_id, max_loan_sec)
        db.update(BOOKS, {"status": '1'}, {"book_id": book_id})


def _handler_return(user: tuple, book_id: str):
    book = _get_book(book_id)
    book_loan = _get_book_loan(book_id)
    if book is None or book_loan is None:
        return
    user_id = int(user[0])
    loan_user_id = int(book_loan[6])
    is_loan = is_on_loan(int(book_id)) or is_loan_suspended(int(book_id))
    if is_loan and user_id == loan_user_id:
        db = LibraryDatabase()
        next_book_status = '0' if is_on_loan(int(book_id)) else '3'
        db.update(BOOKS, {"status": next_book_status}, {"book_id": book_id})
        now = str(time.time())
        loan_id = str(book_loan[0])
        db.update(BOOK_LOANS, {"actual_return_date": now,
                  "is_returned": '1'}, {"loan_id": loan_id})


def _get_book(book_id: str) -> Optional[tuple]:
    db = LibraryDatabase()
    books = db.select(BOOKS, {"book_id": book_id})
    if books == []:
        return None
    return books[0]


def _get_book_loan(book_id: str) -> Optional[tuple]:
    db = LibraryDatabase()
    books = db.select(BOOK_LOANS, {"book_id": book_id}, {"loan_id": "DESC"})
    if books == []:
        return None
    return books[0]


if __name__ == "__main__":
    manage_book_data((4, "new", "new"), '1', "loan")
