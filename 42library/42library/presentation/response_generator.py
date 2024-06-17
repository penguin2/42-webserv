class ResponseGenerator:
    def __init__(self):
        self.__headers = {}
        self.__title = "42Library"
        self.__html_body = ""

    def insert_header(self, key: str, value: str):
        self.__headers[key] = value

    def set_title(self, title):
        self.__title = title

    def append_body(self, body):
        self.__html_body += body

    def generate_headers(self):
        for key, value in self.__headers.items():
            print(f"{key}: {value}")
        print("")

    def generate_page(self):
        self.generate_headers()
        print("<!DOCTYPE html>")
        print('<html lang="ja">')
        print("<head>")
        print('<meta charset="UTF-8">')
        print(f'<title>{self.__title}</title>')
        print('<link rel="stylesheet" href="/static/styles.css">')
        print("</head>")
        print("<body>")
        print(self.__html_body)
        print("</body>")
        print("</html>")


def main():
    generator = ResponseGenerator()
    generator.append_body("<p>new</p>")
    generator.set_title("NEW TITILE")
    generator.insert_header("Location", "/")
    generator.generate_page()


if __name__ == "__main__":
    main()
