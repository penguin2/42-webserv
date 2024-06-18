from presentation.response_generator import ResponseGenerator
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from presentation.content_creator import ContentCreator
from presentation.content_creator import create_simple_book
import cgi

MAXIMUM_NUMBER_OF_BOOKS_PER_PAGE = 4


def generate_index_py_response(user: tuple):
    generator = ResponseGenerator()
    generator.insert_header("Content-Type", "text/html")
    generator.append_body(f"<p>USER: {user[1]}!!</p>")
    generator.append_body('<a href="/logout.html">LOGOUT</a>\n')
    generator.append_body(_create_book_list())
    generator.generate_page()


def _get_page_number_from_query_string() -> int:
    form = cgi.FieldStorage()
    # QUERY_STRINGから'p'というkeyを取得
    page_number = form.getvalue("p")
    if page_number is None:
        page_number = 1
    page_number = int(page_number)
    if page_number <= 0:
        page_number = 1

    return page_number


def _create_book_list() -> str:
    db = LibraryDatabase()
    books = db.select(BOOKS, order_by={"book_id": "DESC"})
    page_number = _get_page_number_from_query_string()

    book_creator = ContentCreator(page_number, books, create_simple_book)
    page_href = book_creator.create_page_href("/42library/index.py")
    book_contents = book_creator.create_contents()
    return page_href + '<div class=books_list>' + book_contents + '</div>'


if __name__ == "__main__":
    generate_index_py_response((1, 2, 3, 34))
