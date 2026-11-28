# Task 1 — OS: Banker's Algorithm (Deadlock Avoidance)

## Problem Statement

Implement the **Banker's Algorithm** for a single-instance resource system.
Given `n` processes and `m` resource types with the current `Allocation`
matrix, `Max` claim matrix and `Available` vector, your program must:

1. Compute the `Need` matrix (`Need = Max - Allocation`).
2. Run the **Safety Algorithm** and report whether the system is in a
   *safe state*, printing a valid safe sequence if one exists.
3. Process a **resource request** vector for one chosen process:
   - reject it if `request > need` or `request > available` (invalid claim),
   - otherwise simulate the allocation and grant it **only if** the
     resulting state is still safe; roll back otherwise.

### Input format (stdin)
```
n m
allocation row 1        (m integers)
...
allocation row n
max row 1                (m integers)
...
max row n
available                (m integers)
request                  (m integers, all -1 to skip the request step)
```

### Example
```
Input:
4 3
0 1 0
2 0 0
3 0 2
2 1 1
7 5 3
3 2 2
9 0 2
2 2 2
3 3 2
1 0 2

Output:
Need matrix:
P1 1 2 2
P2 1 2 2
P3 6 0 0
P4 0 1 1
System is SAFE. Safe sequence: P2 P4 P1 P3
Request (1 0 2) for P2: GRANTED. New safe sequence: ...
```

## Approach

1. **Need matrix:** computed once as `Max - Allocation` per cell.
2. **Safety algorithm:** maintain a `work` copy of `Available` and a
   `finish[]` flag per process. Repeatedly find a process `i` with
   `finish[i] == false` and `Need[i][j] <= work[j]` for every resource
   `j`; simulate it finishing by adding `Allocation[i]` to `work` and
   mark it finished. If a full pass finds no such process, the system is
   **unsafe** iff some process is still unfinished.
3. **Request handling:** validate against `Need` and `Available` first
   (a process may never claim more than it declared). Then temporarily
   apply `Available -= request`, `Allocation += request`,
   `Need -= request` and re-run the safety algorithm; accept only if the
   new state is safe, otherwise **roll back** the tentative allocation.

**Why it works:** a state is safe iff some ordering exists in which every
process can obtain its maximum claim and terminate — the safety check
exhaustively searches for such an ordering, so granting a request that
keeps the state safe can never lead to deadlock.

## Complexity

- Safety check: `O(n^2 * m)` worst case (up to `n` passes, each scanning
  `n` processes over `m` resources).
- Request: same order of magnitude (one extra safety run), plus `O(m)`
  validation.
- Space: `O(n * m)` for the matrices plus `O(n + m)` scratch arrays.
