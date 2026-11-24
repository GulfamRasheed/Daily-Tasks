# Task 1 — DSA: Longest Substring Without Repeating Characters

## Problem Statement

Given a string `s`, find the length of the longest substring that contains no repeated characters.

### Example 1
```
Input:  s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with length 3.
```

### Example 2
```
Input:  s = "bbbbb"
Output: 1
Explanation: The answer is "b", with length 1.
```

### Example 3
```
Input:  s = "pwwkew"
Output: 3
Explanation: The answer is "wke", with length 3.
Note that "pwke" is a subsequence, not a substring.
```

### Constraints
- `0 <= s.length <= 5 * 10^4`
- `s` consists of English letters, digits, symbols and spaces.

## Approach

Sliding window with a hash map (dictionary) storing the last seen index of each character.

1. Keep a window `[left, right]` that always holds unique characters.
2. Move `right` one step at a time. If `s[right]` was seen at an index
   `>= left`, the window now contains a duplicate — jump `left` to
   `last_index + 1` (never move it backwards).
3. Update the last-seen index of `s[right]` and record
   `max_len = max(max_len, right - left + 1)`.

Each character enters and leaves the window at most once, so the scan
is linear.

## Complexity
- **Time:** O(n) — single pass over the string.
- **Space:** O(min(n, Σ)) — hash map of seen characters (Σ = alphabet size).
