from business_logic.sessions_utils import SESSION_ID_KEY
from presentation.response_generator import ResponseGenerator


def generate_no_session_page():
    generator = ResponseGenerator()
    generator.insert_header("Content-Type", "text/html")
    generator.append_body("<p>SESSION is Not Set</p>\n")
    generator.append_body('<a href="/login.html">LOGIN</p>\n')
    generator.generate_page()


def generate_redirect_page(location_value: str):
    generator = ResponseGenerator()
    generator.insert_header("Location", location_value)
    generator.generate_headers()


def generate_redirect_to_index_py(session_id: str, host: str, max_age: int = 60):
    generator = ResponseGenerator()
    generator.insert_header("Location", f"http://{host}/42library/index.py")
    generator.insert_header("Status", "302 Found")
    generator.insert_header("Content-Type", "text/html")
    generator.insert_header(
        "Set-Cookie", f"{SESSION_ID_KEY}={session_id}; Max-Age={max_age}"
    )
    generator.append_body("<p>REDIRECT</p>")
    generator.generate_page()


def generate_logout_page(host: str):
    generator = ResponseGenerator()
    generator.insert_header("Location", f"http://{host}/index.html")
    generator.insert_header("Status", "302 Found")
    generator.insert_header("Content-Type", "text/html")
    generator.insert_header("Set-Cookie", f"{SESSION_ID_KEY}; Max-Age=-1")
    generator.append_body("<p>REDIRECT</p>")
    generator.generate_page()


if __name__ == "__main__":
    generate_no_session_page()
