import math
from typing import Callable


def convert_book_status_to_string(status: int) -> str:
    if status == 0:
        return "貸出可能"
    elif status == 1:
        return "貸出中"
    elif status == 2:
        return "貸出停止"
    else:
        return "削除可能"


def create_book_detail_html(book: tuple) -> str:
    response = f"""
        <div class="book">
            <ul>
                <img src="/images/{book[1]}.png" alt="{book[1]}">
                <li>Title: {book[1]}</li>
                <li>Status: {convert_book_status_to_string(book[2])}</li>
                <li>MaxLoan: {book[2]}秒</li>
                <li>ISBN: {book[3]}</li>
                <li>Owner: {book[4]}</li>
            </ul>
        </div>
    """
    return response


def create_book_html(book: tuple) -> str:
    response = f"""
        <div class="book">
            <ul>
            <img src="/images/{book[0]}.png" alt="{book[1]}">
            <li>Title: {book[1]}</li>
            </ul>
        </div>
    """
    return response


class ContentCreator:
    """
    contents_per_row * contnts_per_colのコンテンツ一覧HTMLを生成する
    """

    def __init__(
        self,
        page_number,  # 現在何ページ目なのかを指定
        contents,  # 複数のコンテンツ(tuple)のList
        func: Callable[[tuple], str],  # データを処理してHTMLを返す関数
        contents_per_row=3,  # コンテンツ一覧の一行のコンテンツ数
        contents_per_col=3,  # コンテンツ一覧の一列のコンテンツ数
    ):
        self._page_number: int = page_number
        self._contents: list = contents
        self._func: Callable[[tuple], str] = func
        self._contents_per_row: int = contents_per_row
        self._contents_per_col: int = contents_per_col
        self._contents_per_page: int = contents_per_row * contents_per_col
        self._max_page_number: int = math.ceil(len(contents) / self._contents_per_page)

    def _slice(self, contents: list, start_idx: int, end_idx: int) -> list[tuple]:
        len_contents = len(contents)
        if len_contents <= start_idx:
            return []
        if len_contents < end_idx:
            end_idx = len(contents)
        return contents[start_idx:end_idx]

    def _create_pre_page_href(self, path: str) -> str:
        if self._page_number <= 1:
            href = ""
        elif self._max_page_number < self._page_number:
            href = f'href="{path}?p={self._max_page_number}"'
        else:
            href = f'href="{path}?p={self._page_number - 1}"'
        return f'<a {href} class="button_page">Previos</a>'

    def _create_next_page_href(self, path: str) -> str:
        if self._page_number < self._max_page_number:
            href = f'href="{path}?p={self._page_number + 1}"'
        else:
            href = ""
        return f'<a {href} class="button_page">Next</a>'

    def create_page_href(self, path: str) -> str:
        len_content = len(self._contents)
        pre_page_href = self._create_pre_page_href(path)
        next_page_href = self._create_next_page_href(path)

        page_start = (self._page_number - 1) * self._contents_per_page + 1
        page_start = len_content if (len_content < page_start) else page_start

        page_end = self._page_number * self._contents_per_page
        page_end = len_content if (len_content < page_end) else page_end
        page_href = f"""
            <div class="pagination">
                {pre_page_href}
                <p>{page_start} - {page_end} of in {len_content} in total</p>
                {next_page_href}
            </div>
        """
        return page_href

    def create_contents(self) -> str:
        # if (page_number = 1, contents_per_page = 4) -> 0
        # if (page_number = 2, contents_per_page = 4) -> 4
        # if (page_number = 3, contents_per_page = 4) -> 8
        base_number = (self._page_number - 1) * self._contents_per_page

        contents_list: list[list[tuple]] = []
        for i in range(self._contents_per_col):
            start_idx = self._contents_per_row * i + base_number
            end_idx = self._contents_per_row * (i + 1) + base_number
            contents_list.append(self._slice(self._contents, start_idx, end_idx))

        html: str = ""
        for contents in contents_list:
            html += '<div class="contents">'
            html += "".join([self._func(content) for content in contents])
            html += "</div>"
        return html
