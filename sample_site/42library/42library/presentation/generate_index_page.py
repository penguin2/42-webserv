from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from business_logic.sessions_utils import SESSION_ID_KEY
from presentation.content_creator import ContentCreator
from business_logic.get_value_from_qstring import get_page_number_from_qstring


def generate_index_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    db = LibraryDatabase()
    books = db.select(BOOKS, order_by={"book_id": "DESC"})
    page_number = get_page_number_from_qstring()
    book_content_creator = ContentCreator(page_number, books, _create_book_html)
    book_contents = book_content_creator.create_contents()
    page_href = book_content_creator.create_page_href("/42library/index.py")

    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.set_html_header(user[1])
    body = f"""
        {page_href}
        <div class=books_list>{book_contents}</div>
    """
    builder.append_body(body)
    builder.generate_page()


def _create_book_html(book: tuple) -> str:
    response = f"""
        <div class="book">
            <a href="/42library/book_detail.py?book_id={book[0]}">
                <ul>
                <li><img src="/42library/images/{book[0]}.png" alt="{book[1]}"></li>
                <li>{book[1]}</li>
                </ul>
            </a>
        </div>
    """
    return response


if __name__ == "__main__":
    generate_index_page((1, 2, 3, 34), 1)
