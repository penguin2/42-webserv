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


def get_status_code_and_connection_from_response(response: str) -> tuple[Optional[int], str]:
    """
    if (Connection disconnected) -> (None, "close")
    elif (has Connection Header in response) -> (status_code, ConnectionHeaderValue)
    else -> (status_code, "")
    """
    if len(response) == 0:
        return (None, "close")
    response_lines = response.split('\r\n')
    status_line = response_lines[0]
    status_code = int(status_line.split()[1])
    for line in response_lines[1:]:
        key, *value = line.split(":")
        if key.lower() == "connection":
            return (status_code, "".join(value).strip())
    return (status_code, "")
