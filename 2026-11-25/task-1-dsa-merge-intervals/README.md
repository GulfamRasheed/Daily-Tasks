# Task 1 — DSA: Merge Intervals

## Problem Statement

Given a list of intervals `intervals` where `intervals[i] = [start, end]`, merge all overlapping intervals and return an array of the non-overlapping intervals that cover all the intervals in the input.

**Examples**

1. `intervals = [[1,3],[2,6],[8,10],[15,18]]`
   Output: `[[1,6],[8,10],[15,18]]`
   - `[1,3]` and `[2,6]` overlap → `[1,6]`
2. `intervals = [[1,4],[4,5]]`
   Output: `[[1,5]]` (touching intervals are merged)

## Approach

1. Sort the intervals by their start value.
2. Iterate through the sorted list, keeping the current merged interval.
3. For each interval:
   - If it starts after the current merged interval ends → no overlap: push the current merged interval to the result and start a new one.
   - Otherwise → overlap: extend the current merged interval's end to `max(current_end, interval_end)`.
4. Push the last remaining merged interval.

**Why it works:** sorting by start guarantees that once a merged interval's end is passed, no later interval can overlap with earlier ones.

## Complexity

- Time: `O(n log n)` — dominated by the sort; the merge pass is `O(n)`.
- Space: `O(n)` for the output (excluding sort overhead; `O(log n)` extra for Timsort).
