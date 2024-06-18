from presentation.response_generator import ResponseGenerator
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from presentation.content_creator import ContentCreator
from presentation.content_creator import create_simple_book
import cgi

MAXIMUM_NUMBER_OF_BOOKS_PER_PAGE = 4


def generate_index_py_response(user: tuple):
    generator = ResponseGenerator()
    db = LibraryDatabase()
    books = db.select(BOOKS, order_by={"book_id": "DESC"})
    page_number = _get_page_number_from_query_string()
    book_creator = ContentCreator(page_number, books, create_simple_book, 3, 4)
    book_contents = book_creator.create_contents()
    page_href = book_creator.create_page_href("/42library/index.py")

    generator.insert_header("Content-Type", "text/html")
    generator.append_body('<header>')
    generator.append_body(
        f'<div class="container"><a class="button">{user[1]}</a></div>')
    generator.append_body(page_href)
    generator.append_body(
        '<div class="container"><a href="/logout.html" class="button">LOGOUT</a></div>\n')
    generator.append_body('</header>')
    generator.append_body(f"<div class=books_list>{book_contents}</div>")
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


if __name__ == "__main__":
    generate_index_py_response((1, 2, 3, 34))
