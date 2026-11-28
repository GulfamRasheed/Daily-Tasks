def edit_distance(word1, word2):
    if len(word1) < len(word2):
        word1, word2 = word2, word1

    m, n = len(word1), len(word2)
    prev = list(range(n + 1))
    curr = [0] * (n + 1)

    for i in range(1, m + 1):
        curr[0] = i
        for j in range(1, n + 1):
            if word1[i - 1] == word2[j - 1]:
                curr[j] = prev[j - 1]
            else:
                curr[j] = 1 + min(prev[j], curr[j - 1], prev[j - 1])
        prev, curr = curr, prev

    return prev[n]


def main():
    tests = [
        ("horse", "ros", 3),
        ("intention", "execution", 5),
        ("", "abc", 3),
        ("same", "same", 0),
        ("sunday", "saturday", 3),
        ("", "", 0),
        ("ab", "ba", 2),
    ]
    for w1, w2, expected in tests:
        got = edit_distance(w1, w2)
        status = "OK" if got == expected else "FAIL"
        print(f"{status}: edit_distance({w1!r}, {w2!r}) = {got} (expected {expected})")


if __name__ == "__main__":
    main()
