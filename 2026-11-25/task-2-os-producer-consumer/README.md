# Task 2 — OS/System Programming: Producer–Consumer with Semaphores

## Problem Statement

Implement the classic **bounded-buffer (producer–consumer)** problem:

- A fixed-size buffer holds items produced by `PRODUCER` threads and consumed by `CONSUMER` threads.
- Producers must not write into a full buffer; consumers must not read from an empty buffer.
- Only one thread may access the buffer at a time (mutual exclusion).
- No item may be lost, duplicated, or consumed before it is produced.

**Requirements (POSIX threads, C):**

- Use semaphores for: empty slots (`empty`), filled slots (`full`), and mutual exclusion (`mutex`).
- Correct synchronization under any interleaving: no race conditions, no deadlock.
- Program runs to completion and prints a consistent log (e.g. `P: put 5`, `C: got 5`).

## Approach

Three semaphores coordinate the threads:

| Semaphore | Initialized to | Meaning |
|-----------|----------------|---------|
| `empty`   | `BUFFER_SIZE`  | number of free slots |
| `full`    | 0              | number of filled slots |
| `mutex`   | 1              | binary lock for buffer access |

- **Producer:** `wait(empty)` → `wait(mutex)` → write item → `signal(mutex)` → `signal(full)`
- **Consumer:** `wait(full)` → `wait(mutex)` → read item → `signal(mutex)` → `signal(empty)`

Ordering matters: `empty`/`full` are waited *before* `mutex` so a blocked producer/consumer never holds the lock.

## Complexity

- Time: `O(P + C)` per item transferred (each put/get is `O(1)` buffer work plus `O(1)` semaphore ops).
- Space: `O(BUFFER_SIZE)` for the circular buffer.

## Build & Run

```sh
gcc solution.c -o solution -pthread && ./solution
```
