from persistence.library_database import LibraryDatabase
from persistence.library_database import BOOKS, BOOK_LOANS
import time


def return_book(book_id: int) -> bool:
    db = LibraryDatabase()
    books = db.select(BOOKS, {"book_id": str(book_id)})
    if not books:
        return False
    book = books[0]
    next_status = 0
    if book[2] == 2:
        # if (current_status == 貸出停止): next_status = 削除可能
        next_status = 3
    db.update(BOOKS, {"status": str(next_status)}, {"book_id": str(book_id)})
    now = int(time.time())
    db.update(
        BOOK_LOANS,
        {"actual_return_date": str(now), "is_returned": "1"},
        {"book_id": str(book_id)},
    )
    return True


def loan_book(book_id: int, user_id: int) -> bool:
    db = LibraryDatabase()
    books = db.select(BOOKS, {"book_id": str(book_id)})
    if not books:
        return False
    book = books[0]
    if int(book[2]) != 0:
        # if (図書が貸出可能ではない): return False
        return False
    db.update(BOOKS, {"status": "1"}, {"book_id": str(book_id)})
    max_loan_sec = int(book[3])
    db.insert_book_loan(book_id, user_id, max_loan_sec)

    return True


if __name__ == "__main__":
    db = LibraryDatabase()
    db.delete(BOOK_LOANS, {"loan_id": "1"})
    db.insert_book_loan(1, 1)
    assert loan_book(1, 1) is True
    assert loan_book(1, 1) is False
    assert return_book(1) is True
