import math
from types import FunctionType


def convert_book_status_to_string(status: int) -> str:
    if status == 0:
        return "貸出可能"
    elif status == 1:
        return "貸出中"
    elif status == 2:
        return "貸出停止"
    else:
        return "削除可能"


def create_book(book: tuple) -> str:
    response = '<div class="book">'
    response += "<ul>"
    response += f'<img src="/images/{book[1]}.png" alt="{book[1]}">'
    response += f"<li>Title: {book[1]}</li>"
    response += f"<li>Status: {convert_book_status_to_string(book[2])}</li>"
    response += f"<li>MaxLoan: {book[2]}秒</li>"
    response += f"<li>ISBN: {book[3]}</li>"
    response += f"<li>Owner: {book[4]}</li>"
    response += "</ul>"
    response += "</div>"
    return response


def create_simple_book(book: tuple) -> str:
    response = '<div class="book">'
    response += "<ul>"
    response += f'<img src="/images/{book[1]}.png" alt="{book[1]}">'
    response += f"<li>Title: {book[1]}</li>"
    response += "</ul>"
    response += "</div>"
    return response


def slice(contents: list, start_idx: int, end_idx: int) -> list[tuple]:
    len_contents = len(contents)
    if len_contents <= start_idx:
        return []
    if len_contents < end_idx:
        end_idx = len(contents)
    return contents[start_idx: end_idx]


class ContentCreator:
    def __init__(self, page_number, contents, fptr, num_of_contents=3, num_of_lines=3):
        self._page_number: int = page_number
        self._contents: list = contents
        self._fptr: FunctionType = fptr
        self._num_of_contents: int = num_of_contents
        self._num_of_lines: int = num_of_lines
        self._contents_per_page: int = num_of_contents * num_of_lines

    def _create_pre_page_href(self, path: str) -> str:
        pre_page_href = ""
        if 1 < self._page_number:
            pre_page_number = self._page_number - 1
            pre_page_href = f'<a href="{path}?p={pre_page_number}">Previos</a>'
        return pre_page_href

    def _create_next_page_href(self, path: str) -> str:
        len_contents = len(self._contents)
        max_page_number: int = math.ceil(
            len_contents / self._contents_per_page)
        next_page_href = ""
        if self._page_number < max_page_number:
            next_page_number = self._page_number + 1
            next_page_href = f'<a href="{path}?p={next_page_number}">Next</a>'
        return next_page_href

    def create_page_href(self, path: str) -> str:
        len_content = len(self._contents)
        pre_page_href = self._create_pre_page_href(path)
        next_page_href = self._create_next_page_href(path)

        page_start = (self._page_number - 1) * self._contents_per_page + 1
        page_end = self._page_number * self._contents_per_page
        page_end = len_content if (len_content < page_end) else page_end
        page_data = f"Displaying entries {page_start} - {page_end} of in {len_content} in total"
        return pre_page_href + page_data + next_page_href

    def create_contents(self) -> str:
        # if (1, 4) -> 0
        # if (2, 4) -> 4
        # if (3, 4) -> 8
        base_number = (self._page_number - 1) * self._contents_per_page

        contents_list = []
        for i in range(self._num_of_lines):
            # (1, 1, 0) -> 0
            # (2, 1, 0) -> 0
            start_idx = self._num_of_contents * i + base_number
            end_idx = self._num_of_contents * (i + 1) + base_number
            contents_list.append(slice(self._contents, start_idx, end_idx))

        contents = ""
        for content_list in contents_list:
            contents += "".join(
                [self._fptr(content) for content in content_list]
            )
        return contents
