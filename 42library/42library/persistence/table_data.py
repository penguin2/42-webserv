"""
サンプルサイトで使用するテーブル・外部キーの定義
"""

USERS_TABLE: dict[str, str] = {
    "user_id": "INTEGER PRIMARY KEY AUTOINCREMENT",
    "user_name": "TEXT UNIQUE NOT NULL",
    "password": "TEXT NOT NULL",
}


BOOKS_TABLE: dict[str, str] = {
    "book_id": "INTEGER PRIMARY KEY AUTOINCREMENT",
    "title": "TEXT NOT NULL",
    "status": "INTEGER NOT NULL DEFAULT 0",
    "max_loan_sec": "INTEGER NOT NULL DEFAULT 3600",
    "isbn": "TEXT NOT NULL",
    "owner_user_id": "INTEGER NOT NULL",
}

SESSIONS_TABLE: dict[str, str] = {
    "session_id": "TEXT PRIMARY KEY",
    "start_date": "INTEGER DEFAULT (strftime('%s', 'now'))",
    "end_date": "INTEGER DEFAULT (strftime('%s', 'now', '+1 days'))",
    "user_id": "INTEGER NOT NULL",
}

BOOK_LOANS_TABLE: dict[str, str] = {
    "loan_id": "INTEGER PRIMARY KEY AUTOINCREMENT",
    "loan_start_date": "INTEGER DEFAULT (strftime('%s', 'now'))",
    "checkout_date": "INTEGER DEFAULT (strftime('%s', 'now', '+7 days'))",
    "actual_return_date": "INTEGER",
    "is_returned": "INTEGER DEFAULT 0",
    "book_id": "INTEGER NOT NULL",
    "loan_user_id": "INTEGER NOT NULL",
}

USERS_FOREIGN_KEYS: dict[str, tuple[str, str]] = {}

BOOKS_FOREIGN_KEYS: dict[str, tuple[str, str]] = {"owner_user_id": ("users", "user_id")}

SESSIONS_FOREIGN_KEYS: dict[str, tuple[str, str]] = {"user_id": ("users", "user_id")}

BOOK_LOANS_FOREIGN_KEYS: dict[str, tuple[str, str]] = {
    "book_id": ("books", "book_id"),
    "loan_user_id": ("users", "user_id"),
}

USERS = "users"
BOOKS = "books"
SESSIONS = "sessions"
BOOK_LOANS = "book_loans"
