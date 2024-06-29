from presentation.html_builder import HtmlBuilder
from business_logic.sessions_utils import SESSION_ID_KEY

add_book_body = """
<form id="add_book_form">
    <div class="form-container">
        <label for="title">TITLE:</label>
        <input type="text" id="title" name="title" required maxlength="50">
    </div>

    <div class="form-container">
        <label for="max_loan_sec">Max Loan:</label>
        <input type="number" id="max_loan_sec" name="max_loan_sec" value="3600" style="text-align:right">sec
    </div>

    <div class="form-container">
        <label for="isbn">ISBN:</label>
        <input type="text" id="isbn" name="isbn" required>
    </div>

    <div class="form-container">
        <label for="book_image">Book Image(.png):
            <input type="file" id="book_image" name="book_image" accept=".png" required>
            <div id="show_book_image" style="margin: 0;"></div>
        </label>
    </div>

	
    <div>
        <div id="request_error_message" style="margin: 0; color: red; text-align: center;"></div>
        <button type="submit" class="add_book_button">Add New Book</button>
    </div>
</form>
<script src="/42library/static/add_book.js"></script>
"""


def generate_add_book_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder(style_file="/42library/static/add_book_styles.css")
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.set_html_header(user[0], user[1])

    builder.append_body(add_book_body)
    builder.generate_page()


if __name__ == "__main__":
    generate_add_book_page((1, 2, 3, 4), "dfadsf")
