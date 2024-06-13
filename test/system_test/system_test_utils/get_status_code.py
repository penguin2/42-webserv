from typing import Optional


def get_status_code_from_response(response: Optional[str]) -> Optional[int]:
    """
    if (Connection disconnected) -> None
    else -> status_code
    """
    if response is None:
        return None
    if len(response) == 0:
        return None
    response_lines = response.split("\r\n")
    status_line = response_lines[0]
    status_code = int(status_line.split()[1])
    return status_code
