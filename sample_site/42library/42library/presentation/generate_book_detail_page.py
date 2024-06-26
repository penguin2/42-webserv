from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from business_logic.sessions_utils import SESSION_ID_KEY
from persistence.table_data import BOOKS
from presentation.create_book_detail_html import create_book_detail_html
import cgi
from typing import Optional


def generate_book_detail_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.set_html_header(user[1])

    db = LibraryDatabase()
    book_id = _get_book_id_from_query_string()
    try:
        books = db.select(BOOKS, conditions={"book_id": str(book_id)})
        book = books[0]
        book_detail = create_book_detail_html(user, book)
    except Exception:
        book_detail = """
        <p style="text-align:center; font-size:50px;">Book is not exist.</p>
        <a href="/42library/" style="text-align:center;font-size:50px;">
            <p>RETURN</p>
        </a>
        """

    builder.append_body(book_detail)
    builder.generate_page()


def _get_book_id_from_query_string() -> Optional[int]:
    form = cgi.FieldStorage()
    book_id = form.getvalue("book_id")
    try:
        book_id = int(book_id)
    except Exception:
        book_id = None

    return book_id


if __name__ == "__main__":
    generate_book_detail_page((1, 2, 3, 34), "dfa")
