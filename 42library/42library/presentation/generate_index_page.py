from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from business_logic.sessions_utils import SESSION_ID_KEY
from presentation.content_creator import ContentCreator
from presentation.content_creator import create_book_html
import cgi


def generate_index_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    db = LibraryDatabase()
    books = db.select(BOOKS, order_by={"book_id": "DESC"})
    page_number = _get_page_number_from_query_string()
    book_content_creator = ContentCreator(page_number, books, create_book_html)
    book_contents = book_content_creator.create_contents()
    page_href = book_content_creator.create_page_href("/42library/index.py")

    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    body = f"""
        <header>
            <div class="container">
                <a class="button">{user[1]}</a>
            </div>
            {page_href}
            <div class="container">
                <a href="/logout.html" class="button">LOGOUT</a>
            </div>
        </header>
        <div class=books_list>{book_contents}</div>
    """
    builder.append_body(body)
    builder.generate_page()


def _get_page_number_from_query_string() -> int:
    """
    QUERY_STRINGから'p'というkeyを取得
    'p'が存在しない・不正な値の場合は1ページ目を表示させる
    """
    form = cgi.FieldStorage()
    page_number = form.getvalue("p")
    try:
        page_number = int(page_number)
    except Exception:
        page_number = 1
    page_number = 1 if (page_number <= 0) else page_number

    return page_number


if __name__ == "__main__":
    generate_index_page((1, 2, 3, 34), 1)
