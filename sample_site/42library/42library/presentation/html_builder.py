class HtmlBuilder:
    def __init__(self):
        self.__headers = {}
        self.__title = "42Library"
        self.__html_body = ""
        self.__html_header = ""

    def insert_header(self, key: str, value: str):
        self.__headers[key] = value

    def set_title(self, title):
        self.__title = title

    def append_body(self, body):
        self.__html_body += body

    def set_html_header(self, user_name: str):
        self.__html_header = f"""
        <header>
            <div class="container">
                <a href="/42library/index.py" class="button">42Library</a>
            </div>
            <div class="container">
                <a class="button">{user_name}</a>
            </div>
            <div class="container">
                <a href="/42library/html/logout.html" class="button">LOGOUT</a>
            </div>
        </header>
        """

    def generate_headers(self):
        for key, value in self.__headers.items():
            print(f"{key}: {value}")
        print("")

    def generate_page(self):
        self.generate_headers()
        html = f"""
            <!DOCTYPE html>
            <html lang="ja">
                <head>
                    <meta charset="UTF-8">
                    <title>{self.__title}</title>
                    <link rel="stylesheet" href="/42library/static/cgi_styles.css">
                </head>
                <body>
                    {self.__html_header}
                    {self.__html_body}
                </body>
            </html>
        """
        print(html)


def main():
    builder = HtmlBuilder()
    builder.append_body("<p>new</p>")
    builder.set_title("NEW TITILE")
    builder.insert_header("Location", "/")
    builder.generate_page()


if __name__ == "__main__":
    main()
