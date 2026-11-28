# Task 1 — OS: Page Replacement Simulator (FIFO / LRU / Optimal)

## Problem Statement

Given a **page reference string** and a fixed number of memory frames,
simulate the three classic page-replacement policies and report the total
number of page faults for each:

1. **FIFO** — replace the page that has been in memory the longest.
2. **LRU** — replace the page that has not been used for the longest time.
3. **Optimal (Belady)** — replace the page that will not be used for the
   longest period in the future (pages never referenced again are the best
   victims).

For every reference the program prints a trace line showing whether the
reference was a **hit** or a **fault** and the resulting frame contents
(`.` marks an empty frame), followed by the fault total per algorithm.

### Input format (stdin)
```
n_frames n_refs
ref_1 ref_2 ... ref_n_refs      (non-negative page numbers, may span lines)
```

### Example
```
Input:
3 20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
```

Output (excerpt — full trace printed by the program):
```
FIFO:
    4: page  2 -> fault frames [ 2  0  1 ]
    5: page  0 -> hit   frames [ 2  0  1 ]
...
FIFO total page faults: 15
LRU total page faults: 12
Optimal total page faults: 9
```

## Approach

All three policies share the same skeleton: scan the reference string once,
look the page up in the frame array, and on a miss pick a victim:

- **FIFO** keeps a circular `next` index into the frame array. Frames are
  filled in order `0..n_frames-1`; once full, `frames[next]` is always the
  oldest-loaded page, so replace it and advance `next = (next + 1) % n_frames`.
  Hits never affect the order — which is exactly FIFO's flaw (it can evict a
  heavily used page, and it suffers from Belady's anomaly).
- **LRU** stores, per frame, the *time stamp* (index) of the last use. On a
  hit the stamp is refreshed; on a miss with no free frame the frame with the
  **smallest** stamp is chosen. A hit is therefore `O(n_frames)` instead of
  `O(1)` (an ordered list/hashmap would give `O(1)` in production kernels).
- **Optimal** looks ahead from the current position: for each resident page
  find the next reference index (or `n_refs` if it never appears again) and
  evict the page whose next use is **farthest** in the future. This is the
  theoretical lower bound on faults but requires future knowledge, so it is
  only usable in simulation.

**Why the fault counts differ:** FIFO ignores recency, LRU uses only the
past, Optimal uses the future. For the classic textbook string above the
counts are 15 / 12 / 9 — LRU beats FIFO and Optimal is the best possible.

## Complexity

Let `n` = number of references, `f` = number of frames.

- FIFO: time `O(n * f)` (linear hit scan per reference), space `O(f)`.
- LRU: time `O(n * f)` (hit scan + linear victim search), space `O(f)`.
- Optimal: time `O(n * f^2)` worst case (for each of `f` frames scan up to
  `n` future references), space `O(f)`.
- Overall output trace is `O(n * f)` characters.
