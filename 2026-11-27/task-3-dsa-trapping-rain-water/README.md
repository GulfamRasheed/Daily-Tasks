# Task 3 — DSA: Trapping Rain Water

## Problem Statement

Given an array of non-negative integers `height` representing an elevation
map where the width of each bar is `1`, compute how much water it can trap
after raining.

**Examples**

1. `height = [0,1,0,2,1,0,1,3,2,1,2,1]`
   Output: `6`
2. `height = [4,2,0,3,2,5]`
   Output: `9`

## Approach (two pointers)

Water above index `i` is `min(left_max[i], right_max[i]) - height[i]`,
where `left_max[i]` / `right_max[i]` are the tallest bars to the left /
right of `i`.

Instead of precomputing both arrays, walk two pointers inward:

1. Start `left = 0`, `right = n-1`, `left_max = right_max = 0`.
2. Whichever side has the **shorter** current bar is processed, because
   the water level at that side is bounded by the *other* side's maximum
   — whichever of `left_max`/`right_max` is smaller is known for sure
   (the taller boundary already exists on the opposite side).
   - If the bar at that pointer is ≥ its running max, update the max
     (no water can be trapped here — it is the new boundary).
   - Otherwise add `running_max - height[pointer]` to the answer.
3. Move that pointer inward and repeat until the pointers meet.

**Why it works:** at each step we only ever finalize an index whose water
level is determined — we never need the unknown side, because the current
pointer's side is the lower of the two boundaries.

## Complexity

- Time: `O(n)` — each pointer moves at most `n` steps, `O(1)` work each.
- Space: `O(1)` extra (no auxiliary arrays).
