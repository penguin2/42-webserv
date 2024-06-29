from persistence.library_database import LibraryDatabase
import cgi


def _file_create(file: str, body: bytes):
    with open(file, "wb") as f:
        f.write(body)


def add_new_book_data(user_id: int) -> bool:
    db = LibraryDatabase()
    form = cgi.FieldStorage()
    title = form.getvalue("title")
    max_loan_sec = form.getvalue("max_loan_sec")
    isbn = form.getvalue("isbn")
    image = form.getvalue("book_image")

    if title is None or max_loan_sec is None or isbn is None or image is None:
        return False

    try:
        book_id = db.insert_book(title, isbn, user_id, max_loan_sec)
        _file_create(f"../images/{book_id}.png", image)
        return True
    except Exception:
        return False
