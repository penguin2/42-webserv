import math


def create_pre_page_href(page_number: int, path: str) -> str:
    pre_page_href = ""
    if 1 < page_number:
        pre_page_number = page_number - 1
        pre_page_href = f'<a href="{path}?p={pre_page_number}">Previos</a>'
    return pre_page_href


def create_next_page_href(page_number: int, path: str, len_content: int, content_per_page: int) -> str:
    max_page_number: int = math.ceil(len_content / content_per_page)
    next_page_href = ""
    if page_number < max_page_number:
        next_page_number = page_number + 1
        next_page_href = f'<a href="{path}?p={next_page_number}">Next</a>'
    return next_page_href


def create_page_href(page_number: int, path: str, len_content: int, content_per_page) -> str:
    pre_page_href = create_pre_page_href(page_number, path)
    next_page_href = create_next_page_href(
        page_number, path, len_content, content_per_page)

    page_start = (page_number - 1) * content_per_page + 1
    page_end = page_number * content_per_page
    page_end = len_content if (len_content < page_end) else page_end
    page_data = f"Displaying entries {page_start} - {page_end} of in {len_content} in total"

    return pre_page_href + page_data + next_page_href
