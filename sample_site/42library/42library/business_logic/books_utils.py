from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.library_database import BOOKS


def _get_book_status(book_id: int) -> Optional[int]:
    db = LibraryDatabase()
    books = db.select(BOOKS, {"book_id": str(book_id)})
    if books:
        book = books[0]
        return book[2]
    return None


def is_available_for_loan(book_id: int) -> bool:
    book_status = _get_book_status(book_id)
    if book_status is None:
        return False
    return book_status == 0


def is_on_loan(book_id: int) -> bool:
    book_status = _get_book_status(book_id)
    if book_status is None:
        return False
    return book_status == 1


def is_loan_suspended(book_id: int) -> bool:
    book_status = _get_book_status(book_id)
    if book_status is None:
        return False
    return book_status == 2


def can_delete_book(book_id: int) -> bool:
    book_status = _get_book_status(book_id)
    if book_status is None:
        return False
    return book_status == 3


def convert_book_status_to_string(status: int) -> str:
    if status == 0:
        return "貸出可能"
    elif status == 1:
        return "貸出中"
    elif status == 2:
        return "貸出停止"
    else:
        return "削除可能"


if __name__ == "__main__":
    db = LibraryDatabase()
    db.insert_book("TEST", "123", 1)
    books = db.select(BOOKS, {"title": "TEST"})
    book = books[0]
    book_id = book[0]

    assert is_on_loan(book_id) is True
    assert is_available_for_loan(book_id) is False
    assert is_loan_suspended(book_id) is False

    db.delete(BOOKS, {"book_id": str(book_id)})
