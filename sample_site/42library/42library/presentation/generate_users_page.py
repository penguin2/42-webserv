from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from persistence.table_data import USERS
from business_logic.sessions_utils import SESSION_ID_KEY
from presentation.content_creator import ContentCreator
from business_logic.get_value_from_qstring import get_page_number_from_qstring


def generate_users_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    db = LibraryDatabase()
    users = db.select(USERS, order_by={"user_id": "DESC"})
    page_number = get_page_number_from_qstring()
    user_content_creator = ContentCreator(page_number, users, _create_user_html)
    user_contents = user_content_creator.create_contents()
    page_href = user_content_creator.create_page_href("/42library/users.py")

    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.set_html_header(user[1])
    body = f"""
        {page_href}
        <div class=users_list>{user_contents}</div>
    """
    builder.append_body(body)
    builder.generate_page()


def _create_user_html(user: tuple) -> str:
    response = f"""
        <div class="user">
            <a href="/42library/user_detail.py?user_id={user[0]}">
                <p>{user[1]}</p>
            </a>
        </div>
    """
    return response
