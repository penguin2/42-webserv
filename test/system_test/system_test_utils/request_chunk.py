from typing import Generator
import requests
import random


def chunk_body_generator(body: str, max_chunk_size: int = 20) -> Generator:
    """
    文字列をチャンクで送信するためのジェネレータ
    ランダムにチャンクサイズを決め, bodyをスライスしてyield
    """
    sent_bytes = 0
    while sent_bytes < len(body):
        not_sent_bytes = len(body) - sent_bytes
        chunk_size = random.randint(1, max_chunk_size)
        if not_sent_bytes < chunk_size:
            chunk_size = not_sent_bytes
        yield body[sent_bytes: (sent_bytes + chunk_size)]
        sent_bytes += chunk_size


def request_chunk(url: str, body: str) -> requests.Response:
    response = requests.post(url, data=chunk_body_generator(body))
    return response
