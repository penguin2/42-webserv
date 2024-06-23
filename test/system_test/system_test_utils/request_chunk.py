from typing import Generator
import requests


def chunk_body_generator(body: str, chunk_size: int = 3) -> Generator:
    """
    文字列をチャンクで送信するためのジェネレータ
    bodyをスライスしてyield
    """
    sent_bytes = 0
    while sent_bytes < len(body):
        not_sent_bytes = len(body) - sent_bytes
        if not_sent_bytes < chunk_size:
            chunk_size = not_sent_bytes
        yield body[sent_bytes: (sent_bytes + chunk_size)]
        sent_bytes += chunk_size


def request_chunk(url: str, body: str) -> requests.Response:
    response = requests.post(url, data=chunk_body_generator(body))
    return response
