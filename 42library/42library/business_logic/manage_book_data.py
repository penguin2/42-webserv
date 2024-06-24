from typing import Optional
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from persistence.table_data import BOOK_LOANS
from business_logic.books_utils import is_loan_suspended
from business_logic.books_utils import is_on_loan
from business_logic.books_utils import is_available_for_loan
from business_logic.books_utils import can_delete_book
import time
import os


def manage_book_data(user: tuple, book_id: Optional[str], control: Optional[str]):
    handlers = {
        "stop": _handler_stop,
        "delete": _handler_delete,
        "loan": _handler_loan,
        "return": _handler_return
    }
    if (book_id is None) or (control is None):
        return
    try:
        handlers[control](user, int(book_id))
    except Exception:
        pass


def _handler_stop(user: tuple, book_id: int):
    """
    if (図書のオーナーが自分 and 現在誰かが図書を借りている場合):
        図書のstatus = 貸出停止
    """
    book = _get_book(book_id)
    if book is None:
        return
    is_my_book = int(user[0]) == int(book[5])
    if is_my_book and is_on_loan(book_id):
        db = LibraryDatabase()
        db.update(BOOKS, {"status": '2'}, {"book_id": str(book_id)})


def _handler_delete(user: tuple, book_id: int):
    """
    if (図書のオーナーが自分 and
        (現在誰も図書を借りていない or 図書のstatus == 削除可能)):
        DBから図書を削除
        画像ファイルの削除権限を与え、別のDELETE_REQUESTで画像を削除可能にする
    """
    book = _get_book(book_id)
    if book is None:
        return
    is_my_book = int(user[0]) == int(book[5])
    can_delete = is_available_for_loan(book_id) or can_delete_book(book_id)
    if is_my_book and can_delete:
        db = LibraryDatabase()
        db.delete(BOOKS, {"book_id": str(book_id)})
        os.chmod(f"./../images/{book_id}.png", 0o755)


def _handler_loan(user: tuple, book_id: int):
    """
    if (図書が貸出可能):
        book_loansテーブルに新しい行の追加
        図書のstatus = 貸出中
    """
    book = _get_book(book_id)
    if book is None:
        return
    if is_available_for_loan(book_id):
        db = LibraryDatabase()
        user_id = int(user[0])
        max_loan_sec = int(book[3])
        db.insert_book_loan(book_id, user_id, max_loan_sec)
        db.update(BOOKS, {"status": '1'}, {"book_id": str(book_id)})


def _handler_return(user: tuple, book_id: int):
    """
    if ((図書が貸出中 or 貸出停止中) and 自分が借りている):
        図書のstatusを更新
        book_loanテーブルの該当する行を更新
    """
    book = _get_book(book_id)
    book_loan = _get_book_loan(book_id)
    if (book is None) or (book_loan is None):
        return
    is_loan = is_on_loan(book_id) or is_loan_suspended(book_id)
    have_i_loan = int(user[0]) == int(book_loan[6])
    if have_i_loan and is_loan:
        db = LibraryDatabase()
        # if 現在のstatusが貸出中: next_book_status = 貸出可能
        # if 現在のstatusが貸出停止: next_book_status = 削除可能
        next_book_status = 0 if is_on_loan(book_id) else 3
        loan_id = str(book_loan[0])
        db.update(
            BOOKS,
            {"status": str(next_book_status)},
            {"book_id": str(book_id)}
        )
        db.update(
            BOOK_LOANS,
            {"actual_return_date": str(time.time()), "is_returned": '1'},
            {"loan_id": loan_id}
        )


def _get_book(book_id: int) -> Optional[tuple]:
    db = LibraryDatabase()
    books = db.select(BOOKS, {"book_id": str(book_id)})
    if books == []:
        return None
    return books[0]


def _get_book_loan(book_id: int) -> Optional[tuple]:
    db = LibraryDatabase()
    conditions = {"book_id": str(book_id)}
    order_by = {"loan_id": "DESC"}
    books = db.select(BOOK_LOANS, conditions, order_by)
    if books == []:
        return None
    return books[0]


if __name__ == "__main__":
    manage_book_data((4, "new", "new"), '1', "loan")
