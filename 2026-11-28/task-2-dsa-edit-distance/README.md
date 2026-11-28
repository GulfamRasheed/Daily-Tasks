# Task 2 — DSA: Edit Distance (Levenshtein Distance)

## Problem Statement

Given two strings `word1` and `word2`, return the **minimum number of
operations** required to convert `word1` into `word2`. You may perform
exactly one of the following three operations on a character:

- **insert** a character,
- **delete** a character,
- **replace** a character.

**Examples**

1. `word1 = "horse"`, `word2 = "ros"` → Output: `3`
   (replace `r`→`r`? no: `horse` → `rorse` (replace h→r) → `rose` (delete r) → `ros` (delete e))
2. `word1 = "intention"`, `word2 = "execution"` → Output: `5`
3. `word1 = ""`, `word2 = "abc"` → Output: `3` (three inserts)
4. `word1 = "same"`, `word2 = "same"` → Output: `0`

## Approach (dynamic programming)

Let `dp[i][j]` = edit distance between `word1[:i]` and `word2[:j]`
(prefixes of length `i` and `j`).

**Recurrence**

```
dp[0][j] = j            # insert all j characters
dp[i][0] = i            # delete all i characters
if word1[i-1] == word2[j-1]:
    dp[i][j] = dp[i-1][j-1]          # characters match: free
else:
    dp[i][j] = 1 + min(
        dp[i-1][j],      # delete word1[i-1]
        dp[i][j-1],      # insert word2[j-1] before word1[i-1]
        dp[i-1][j-1]     # replace word1[i-1] with word2[j-1]
    )
```

**Optimisation:** row `i` only depends on row `i-1`, so keep two 1-D arrays
(`prev`, `curr`) of length `len(word2) + 1` and swap them each row —
`O(min(m, n))` space after choosing the shorter string as the "columns"
dimension.

**Why it works:** every optimal transformation ends with one of the three
final edits (or a match), so the minimum over those choices plus the
optimal cost of the remaining prefixes is optimal — the standard optimal
substructure argument for edit distance.

## Complexity

- Time: `O(m * n)` where `m = len(word1)`, `n = len(word2)` — every cell of
  the DP table is filled once with `O(1)` work.
- Space: `O(min(m, n))` with the rolling two-row (or single-row with a
  diagonal variable) implementation.
