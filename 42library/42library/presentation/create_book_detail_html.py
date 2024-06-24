from persistence.table_data import USERS
from persistence.table_data import BOOK_LOANS
from business_logic.books_utils import is_on_loan
from business_logic.books_utils import is_loan_suspended
from business_logic.books_utils import is_available_for_loan
from business_logic.books_utils import convert_book_status_to_string
from persistence.library_database import LibraryDatabase


def create_book_detail_html(user: tuple, book: tuple) -> str:
    db = LibraryDatabase()
    owner_user: tuple = db.select(USERS, {"user_id": str(book[5])})[0]
    book_status_str = convert_book_status_to_string(book[2])
    loan_user_data = _create_loan_user_data(book)
    book_loan_data = _create_book_loan_data(user, book)
    html = f"""
    <div class="book_detail">
        <ul>
            <img src="/images/{book[0]}.png" alt="{book[1]}">
            <li data-label="Title:">{book[1]}</li>
            <li data-label="Status:">{book_status_str}</li>
            <li data-label="MaxLoan:">{book[3]}秒</li>
            <li data-label="ISBN:">{book[4]}</li>
            <li data-label="Owner:">{owner_user[1]}</li>
            {loan_user_data}
            <div class="book_loan">{book_loan_data}</div>
        </ul>
    </div>
    """
    return html


def _create_loan_user_data(book: tuple) -> str:
    book_id = int(book[0])
    if is_on_loan(book_id) or is_loan_suspended(book_id):
        loan_user_id: str = str(_get_loan_user_id(book_id))
        db = LibraryDatabase()
        loan_user = db.select(USERS, {"user_id": loan_user_id})[0]
        return f'<li data-label="LoanUser:">{loan_user[1]}</li>'
    return ""


def _create_book_loan_data(user: tuple, book: tuple) -> str:
    user_id = int(user[0])
    book_id = int(book[0])
    book_owner_id = int(book[5])

    # 自分が本の所有者
    if user_id == book_owner_id:
        # 現在貸出中 -> 貸出停止
        if is_on_loan(book_id):
            return _create_loan_href(book_id, "stop", "STOP LOAN BOOK?")
        # 現在貸出停止中
        elif is_loan_suspended(book_id):
            return ""
        # それ以外 -> 図書館から本を削除
        return _create_delete_href(book_id, "DELETE BOOK?")

    # 現在貸出中
    if is_on_loan(book_id) or is_loan_suspended(book_id):
        loan_user_id = _get_loan_user_id(book_id)
        # 自分が借りている
        if int(loan_user_id) == user_id:
            return _create_loan_href(book_id, "return", "RETURN BOOK?")

    elif is_available_for_loan(book_id):
        return _create_loan_href(book_id, "loan", "LOAN BOOK?")

    return ""


def _create_loan_href(book_id: int, control: str, message: str) -> str:
    return f"""
    <a href="/42library/book_loan.py?book_id={book_id}&control={control}">
        {message}
    </a>
    """


def _create_delete_href(book_id: int, message: str) -> str:
    return f"""
    <a href="#" id="delete_book" book_id={book_id}>
        {message}
    </a>
    <script src="/static/delete_book.js"></script>
    """


def _get_loan_user_id(book_id: int) -> int:
    db = LibraryDatabase()
    conditions = {"book_id": str(book_id)}
    order_by = {"loan_id": "DESC"}
    book_loan = db.select(BOOK_LOANS, conditions, order_by)[0]
    loan_user_id = book_loan[6]
    return loan_user_id


if __name__ == "__main__":
    html = create_book_detail_html((3, "new", "new"),
                                   (1, "1", "1", "1", "1", "1"))
    print(html)
