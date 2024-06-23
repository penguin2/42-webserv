from persistence.table_data import USERS
from persistence.table_data import BOOK_LOANS
from business_logic.books_utils import is_on_loan
from business_logic.books_utils import is_loan_suspended
from persistence.library_database import LibraryDatabase


def create_book_detail_html(user: tuple, book: tuple) -> str:
    db = LibraryDatabase()
    owner_user: tuple = db.select(USERS, {"user_id": str(book[5])})[0]
    html = f"""
    <div class="book_detail"><ul>
        <img src="/images/{book[0]}.png" alt="{book[1]}">
        <li data-label="Title:">{book[1]}</li>
        <li data-label="Status:">{_convert_book_status_to_string(book[2])}</li>
        <li data-label="MaxLoan:">{book[3]}秒</li>
        <li data-label="ISBN:">{book[4]}</li>
        <li data-label="Owner:">{owner_user[1]}</li>
        {_create_book_loan(user, book)}
    </ul></div>
    """
    return html


def _convert_book_status_to_string(status: int) -> str:
    if status == 0:
        return "貸出可能"
    elif status == 1:
        return "貸出中"
    elif status == 2:
        return "貸出停止"
    else:
        return "削除可能"


def _create_book_loan(user: tuple, book: tuple) -> str:
    user_id = user[0]
    book_id = book[0]
    book_owner_id = book[5]
    db = LibraryDatabase()

    if user_id == book_owner_id:
        if is_on_loan(book_id):
            book_loan_html = "<p>STOP LOAN BOOK?</p>"
        else:
            book_loan_html = "<p>DELETE BOOK?</p>"
    else:
        if is_on_loan(book_id):
            loan_user: tuple = db.select(
                BOOK_LOANS, {"book_id": book[0]}, {"loan_id": "DESC"}
            )[0]
            book_loan_html = f"<p>{loan_user[1]} is Loan.</p>"
        elif not is_loan_suspended(book_id):
            book_loan_html = "<p>Loan?</p>"

    html = f"""
        <div class="book_loan">
        {book_loan_html}
        </div>
    """
    return html
