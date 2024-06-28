from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from business_logic.sessions_utils import SESSION_ID_KEY
from persistence.table_data import BOOKS
from presentation.create_book_detail_html import create_book_detail_html
from business_logic.get_value_from_qstring import get_id_from_qstring


def generate_book_detail_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.set_html_header(user[0], user[1])

    db = LibraryDatabase()
    book_id = get_id_from_qstring("book_id")
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


if __name__ == "__main__":
    generate_book_detail_page((1, 2, 3, 34), "dfa")
