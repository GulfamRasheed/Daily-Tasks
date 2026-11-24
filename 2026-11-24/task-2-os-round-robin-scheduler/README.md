# Task 2 — OS: Round-Robin CPU Scheduling Simulator

## Problem Statement

Implement a **Round-Robin (RR)** CPU scheduler. Given `n` processes with
their arrival times and burst times, and a time quantum `q`, simulate the
scheduling and compute, for each process:

- **Completion time (CT)** — time at which the process finishes.
- **Turnaround time (TAT)** — `CT - arrival_time`.
- **Waiting time (WT)** — `TAT - burst_time`.

### Input format (stdin)
```
n q
arrival1 burst1
arrival2 burst2
...
```

### Example
```
Input:
4 2
0 5
1 3
2 1
4 4

Output:
PID  AT  BT  CT  TAT  WT
P1    0   5   8    8   3
P2    1   3   9    8   5
P3    2   1   4    2   1
P4    4   4   11   7   3
Average TAT = 6.25, Average WT = 3.00
```

## Approach

1. Keep a **ready queue** of process indices. Processes enter the queue
   in arrival order (a process is enqueued the first time its arrival
   time is reached, including arrivals that happen while the CPU runs).
2. Pop the front process, run it for `min(remaining_burst, q)` time
   units, advancing a global clock.
3. If the process still has remaining burst, push it to the **back** of
   the queue (preemption after the quantum). Otherwise it completes —
   record `CT`, and derive `TAT`/`WT`.
4. Between quanta, enqueue every newly arrived process whose arrival
   time is `<= current time` and which has not been enqueued yet.
5. Repeat until the queue is empty and all processes have arrived.

Note: a process is enqueued at most once in flight, tracked with a
`queued`/`done` flag, so arrivals are never duplicated.

## Complexity
- **Time:** O(total_burst / q * n) in the worst case for queue handling;
  effectively O(total_burst + n log n) with the simple array-based queue
  used here (each unit of burst causes at most one enqueue/dequeue).
- **Space:** O(n) for the process table and the ready queue.
