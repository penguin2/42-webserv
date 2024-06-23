from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from business_logic.sessions_utils import SESSION_ID_KEY
from persistence.table_data import BOOKS
from persistence.table_data import USERS
import cgi
from typing import Optional


def generate_book_detail_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )

    db = LibraryDatabase()
    book_id = _get_book_id_from_query_string()
    try:
        books = db.select(BOOKS, conditions={"book_id": str(book_id)})
        book = books[0]
        book_detail = _create_book_detail_html(book)
    except Exception:
        book_detail = """
        <p style="text-align:center; font-size:50px;">Book is not exist.</p>
        <a href="/42library/" style="text-align:center;font-size:50px;">
            <p>RETURN</p>
        </a>
        """

    body = f"""
        <header>
            <div class="container"><a class="button">{user[1]}</a></div>
            <div class="container">
                <a href="/logout.html" class="button">LOGOUT</a>
            </div>
        </header>
        {book_detail}
    """
    builder.append_body(body)
    builder.generate_page()


def _get_book_id_from_query_string() -> Optional[int]:
    form = cgi.FieldStorage()
    book_id = form.getvalue("book_id")
    try:
        book_id = int(book_id)
    except Exception:
        book_id = None

    return book_id


def _convert_book_status_to_string(status: int) -> str:
    if status == 0:
        return "貸出可能"
    elif status == 1:
        return "貸出中"
    elif status == 2:
        return "貸出停止"
    else:
        return "削除可能"


def _create_book_detail_html(book: tuple) -> str:
    db = LibraryDatabase()
    user: tuple = db.select(USERS, {"user_id": str(book[5])})[0]
    response = f"""
    <div class="book_detail"><ul>
        <img src="/images/{book[0]}.png" alt="{book[1]}">
        <li data-label="Title:">{book[1]}</li>
        <li data-label="Status:">{_convert_book_status_to_string(book[2])}</li>
        <li data-label="MaxLoan:">{book[3]}秒</li>
        <li data-label="ISBN:">{book[4]}</li>
        <li data-label="Owner:">{user[1]}</li>
    </ul></div>
    """
    return response


if __name__ == "__main__":
    generate_book_detail_page((1, 2, 3, 34), 1)
