"""Longest Substring Without Repeating Characters (LeetCode 3)."""


def length_of_longest_substring(s: str) -> int:
    last_index = {}
    left = 0
    max_len = 0

    for right, ch in enumerate(s):
        if ch in last_index and last_index[ch] >= left:
            left = last_index[ch] + 1
        last_index[ch] = right
        max_len = max(max_len, right - left + 1)

    return max_len


if __name__ == "__main__":
    tests = [
        ("abcabcbb", 3),
        ("bbbbb", 1),
        ("pwwkew", 3),
        ("", 0),
        (" ", 1),
        ("dvdf", 3),
        ("au", 2),
        ("tmmzuxt", 5),
    ]
    for s, expected in tests:
        got = length_of_longest_substring(s)
        status = "OK" if got == expected else "FAIL"
        print(f"{status}: length_of_longest_substring({s!r}) = {got} (expected {expected})")
        assert got == expected, f"failed on {s!r}"
    print("All tests passed.")
