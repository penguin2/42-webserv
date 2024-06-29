from presentation.html_builder import HtmlBuilder
from persistence.library_database import LibraryDatabase
from business_logic.sessions_utils import SESSION_ID_KEY
from persistence.table_data import USERS
from presentation.create_user_detail_html import create_user_detail_html
from business_logic.get_value_from_qstring import get_id_from_qstring


def generate_user_detail_page(user: tuple, session_id: str, max_age: int = 30):
    builder = HtmlBuilder()
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.set_html_header(user[0], user[1])

    db = LibraryDatabase()
    user_id = get_id_from_qstring("user_id")
    try:
        users = db.select(USERS, conditions={"user_id": str(user_id)})
        detail_user = users[0]
        user_detail = create_user_detail_html(detail_user)
    except Exception:
        user_detail = """
        <p style="text-align:center; font-size:50px;">User is not exist.</p>
        <a href="/42library/" style="text-align:center;font-size:50px;">
            <p>RETURN</p>
        </a>
        """

    builder.append_body(user_detail)
    builder.generate_page()
