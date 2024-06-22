from business_logic.sessions_utils import SESSION_ID_KEY
from presentation.html_builder import HtmlBuilder


def generate_redirect_page(location_value: str):
    builder = HtmlBuilder()
    builder.insert_header("Location", location_value)
    builder.generate_headers()


def generate_redirect_to_index_py(session_id: str, host: str, max_age: int = 30):
    builder = HtmlBuilder()
    builder.insert_header("Location", f"http://{host}/42library/index.py")
    builder.insert_header("Status", "302 Found")
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    builder.append_body("<p>REDIRECT</p>")
    builder.generate_page()


def generate_redirect_logout_page(host: str):
    builder = HtmlBuilder()
    builder.insert_header("Location", f"http://{host}/index.html")
    builder.insert_header("Status", "302 Found")
    builder.insert_header("Content-Type", "text/html")
    builder.insert_header("Set-Cookie", f"{SESSION_ID_KEY}=; Max-Age=-1")
    builder.append_body("<p>REDIRECT</p>")
    builder.generate_page()


if __name__ == "__main__":
    generate_redirect_logout_page("/logout.html")
