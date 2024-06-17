from presentation.response_generator import ResponseGenerator
from persistence.library_database import LibraryDatabase
from persistence.table_data import BOOKS
from presentation.create_page_href import create_page_href
import cgi

MAXIMUM_NUMBER_OF_BOOKS_PER_PAGE = 4


def generate_index_py_response(user: tuple):
    generator = ResponseGenerator()
    generator.insert_header("Content-Type", "text/html")
    generator.append_body(f"<p>USER: {user[1]}!!</p>")
    generator.append_body('<a href="/logout.html">LOGOUT</a>\n')
    generator.append_body(_create_book_list())
    generator.generate_page()


def _convert_book_status_to_string(status: int) -> str:
    if status == 0:
        return "貸出可能"
    elif status == 1:
        return "貸出中"
    elif status == 2:
        return "貸出停止"
    else:
        return "削除可能"


def _create_book(book: tuple) -> str:
    response = '<div class="book">'
    response += "<ul>"
    response += f'<img src="/images/{book[1]}.png" alt="{book[1]}">'
    response += f"<li>Title: {book[1]}</li>"
    # response += f"<li>Status: {_convert_book_status_to_string(book[2])}</li>"
    # response += f"<li>MaxLoan: {book[2]}秒</li>"
    # response += f"<li>ISBN: {book[3]}</li>"
    # response += f"<li>Owner: {book[4]}</li>"
    response += "</ul>"
    response += "</div>"
    return response


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


def slice_books(books: list[tuple], page_number: int) -> list[list[tuple]]:
    slice_start_idx: int = (page_number - 1) * MAXIMUM_NUMBER_OF_BOOKS_PER_PAGE
    slice_end_idx: int = (page_number * MAXIMUM_NUMBER_OF_BOOKS_PER_PAGE)
    if len(books) <= slice_start_idx:
        return []
    if len(books) < slice_end_idx:
        slice_end_idx = len(books)
    return books[slice_start_idx: slice_end_idx]


def _create_book_list() -> str:
    db = LibraryDatabase()
    books = db.select(BOOKS, order_by={"book_id": "DESC"})

    page_number = _get_page_number_from_query_string()
    page_href = create_page_href(
        page_number, "/42library/index.py", len(books), MAXIMUM_NUMBER_OF_BOOKS_PER_PAGE)
    sliced_books = slice_books(books, page_number)
    book_html_list = [_create_book(book) for book in sliced_books]

    return page_href + '<div class=books>' + "".join(book_html_list) + '</div>'


if __name__ == "__main__":
    generate_index_py_response((1, 2, 3, 34))
