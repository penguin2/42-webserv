from persistence.table_data import BOOKS
from persistence.table_data import BOOK_LOANS
from persistence.library_database import LibraryDatabase
from datetime import datetime


def create_user_detail_html(user: tuple) -> str:
    user_id = user[0]
    user_name = user[1]
    my_books: str = _get_my_books(user_id)
    loan_books: str = _get_loan_books(user_id)
    loan_histories = _get_loan_histories(user_id)
    html = f"""
    <div class="user_detail">
        <p class="user_name">{user_name}</p>
        <div class="book_data_title">
            <p>{user_name}'s BOOKS</p>
            <ul class="book_data">{my_books}</ul>
        </div>
        <div class="book_data_title">
            <p>LOAN BOOKS</p>
            <ul class="book_data">{loan_books}</ul>
        </div>
        <div class="book_data_title">
            <p>LOAN HISTORIES</p>
            <ul class="book_data">{loan_histories}</ul>
        </div>
    </div>
    """
    return html


def _get_my_books(user_id: str) -> str:
    db = LibraryDatabase()
    books = db.select(BOOKS, {"owner_user_id": user_id})

    html = ""
    for book in books:
        book_id = book[0]
        book_name = book[1]
        html += f"""
        <li>
            <a href="/42library/book_detail.py?book_id={book_id}">
                <p>
                    {book_name}
                </p>
            <a>
        </li>
        """

    return html


def _get_loan_books(user_id: str) -> str:
    db = LibraryDatabase()
    conditions = {"loan_user_id": user_id, "is_returned": "0"}
    loans = db.select(BOOK_LOANS, conditions=conditions)

    html = ""
    for loan in loans:
        book_id = loan[5]
        book = db.select(BOOKS, {"book_id": book_id})[0]
        book_name = book[1]
        limit_date = _convert_timestamp_to_date(loan[2])
        html += f"""
        <li>
            <a href="/42library/book_detail.py?book_id={book_id}">
                <p>
                    {book_name}({limit_date})
                </p>
            <a>
        </li>
        """

    return html


def _get_loan_histories(user_id: str) -> str:
    db = LibraryDatabase()
    conditions = {"loan_user_id": user_id, "is_returned": "1"}
    loans = db.select(BOOK_LOANS, conditions=conditions)

    html = ""
    for loan in loans:
        book_id = loan[5]
        book = db.select(BOOKS, {"book_id": book_id})[0]
        book_name = book[1]
        loan_start_date = _convert_timestamp_to_date(loan[1])
        loan_return_date = _convert_timestamp_to_date(loan[3])
        html += f"""
        <li>
            <a href="/42library/book_detail.py?book_id={book_id}">
                <p>
                    {book_name}({loan_start_date} ~ {loan_return_date})
                </p>
            <a>
        </li>
        """

    return html


def _convert_timestamp_to_date(timestamp: float) -> str:

    date_time = datetime.fromtimestamp(timestamp)
    return date_time.strftime("%Y年%m月%d日%H時%M分%S秒")
