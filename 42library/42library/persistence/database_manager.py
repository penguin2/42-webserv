import sqlite3


class DatabaseManager():
    def __init__(self, db_filename: str):
        self.conn = sqlite3.connect(db_filename)

    def __del__(self):
        self.conn.close()

    def _execute(self, sql: str, values: list = []) -> sqlite3.Cursor:
        with self.conn:
            cursor = self.conn.cursor()
            cursor.execute(sql, values)
            return cursor

    def create_table(self,
                     table: str,
                     column_dict: dict[str, str],
                     foreign_keys: dict[str, tuple[str, str]] = {}):
        columns = [
            f'{column_name} {column_data}'
            for column_name, column_data in column_dict.items()
        ]

        if foreign_keys:
            for col, (foreign_table, foreign_column) in foreign_keys.items():
                columns.append(
                    f"""
                    FOREIGN KEY({col})
                    REFERENCES {foreign_table}({foreign_column})
                    """
                )

        self._execute(
            f"""
            CREATE TABLE IF NOT EXISTS {table}
            ({', '.join(columns)});
            """
        )

    def insert(self, table: str, data: dict) -> sqlite3.Cursor:
        placeholders: str = ', '.join('?' * len(data))
        column_names: str = ', '.join(data.keys())
        column_values: list = list(data.values())
        query = f"""
                INSERT INTO {table}
                ({column_names})
                VALUES ({placeholders});
                """
        return self._execute(query, column_values)

    def select(self, table: str,
               conditions: dict[str, str] = {},
               order_by: dict[str, str] = {}):
        query = f'SELECT * FROM {table}'

        if conditions:
            placeholders = [f'{column} = ?' for column in conditions.keys()]
            select_conditions = ' AND '.join(placeholders)
            query += f' WHERE {select_conditions}'

        if order_by:
            order_list = [f'{key} {order}' for key, order in order_by.items()]
            order_string = ', '.join(order_list)
            query += f' ORDER BY {order_string}'

        return self._execute(query, list(conditions.values()))

    def update(self, table: str,
               data_dict: dict[str, str],
               conditions: dict[str, str]) -> sqlite3.Cursor:
        data_placeholders = [f'{column} = ?' for column in data_dict.keys()]
        update_datas = ', '.join(data_placeholders)
        conditions_placeholders = [
            f'{condition} = ?' for condition in conditions.keys()]
        update_conditions = ' AND '.join(conditions_placeholders)
        query = f"""
                UPDATE {table}
                SET {update_datas}
                WHERE {update_conditions}
                """
        return self._execute(query, list(data_dict.values()) + list(conditions.values()))

    def delete(self, table: str, conditions: dict[str, str]) -> sqlite3.Cursor:
        placeholders = [f'{column} = ?' for column in conditions.keys()]
        delete_conditions = ' AND '.join(placeholders)
        query = f"""
                DELETE FROM {table}
                WHERE {delete_conditions}
                """
        return self._execute(query, list(conditions.values()))
