#!/usr/bin/env python3

import socket
import re
import types
import argparse

PATTERN_MSG_VALUE = r"^VAL:(?P<value>\d+\.?\d*)$"
PATTERN_MSG_ALARM = r"^ALM:(?P<state>\d)$"


def connect(host: str, port: int) -> None:
    """
    Connect to target by given `host` and `port` settings.
    """

    print("Try to connect to target...")
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((host, port))
            print("Connected to target")
            fp = sock.makefile("r")

            for line in fp:
                line = line.strip()
                handle_message(line)

            print("Lost connection to target")
    except Exception as exc:
        print(f"Failed to connect to target ({type(exc).__name__})")


def handle_message(message: str):
    """
    Handle each received message or rather line.
    Check for possible message types and handle their information.
    """

    # Check for value message
    match_value = re.match(PATTERN_MSG_VALUE, message)
    if match_value:
        value = match_value.group("value")
        print(f"Current Temperature: {value} °C")

    # Check for alarm message
    match_alarm = re.match(PATTERN_MSG_ALARM, message)
    if match_alarm:
        state = int(match_alarm.group("state"))
        if state == 1:
            print("Target entered alarm state.")
            print("Press button to restart system...")
        elif state == 0:
            print("Target left alarm state.")


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Read messages from target")
    parser.add_argument("host", type=str, help="Host to connect to")
    args = parser.parse_args()

    connect(args.host, 4711)