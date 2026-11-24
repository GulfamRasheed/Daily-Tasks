/*
 * Round-Robin CPU scheduling simulator.
 *
 * Input (stdin):
 *   n q
 *   arrival1 burst1
 *   ...
 *
 * Output: per-process CT, TAT, WT and the averages.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
    int started;   /* has been enqueued at least once */
} Process;

int main(void) {
    int n, q;
    if (scanf("%d %d", &n, &q) != 2 || n <= 0 || q <= 0) {
        fprintf(stderr, "Invalid input: expected 'n q' with n,q > 0\n");
        return 1;
    }

    Process *p = calloc((size_t)n, sizeof(Process));
    int *queue = calloc((size_t)n, sizeof(int)); /* circular, cap = n */
    if (!p || !queue) {
        fprintf(stderr, "Out of memory\n");
        free(p);
        free(queue);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        if (scanf("%d %d", &p[i].arrival, &p[i].burst) != 2) {
            fprintf(stderr, "Invalid input at process %d\n", i + 1);
            free(p);
            free(queue);
            return 1;
        }
        p[i].remaining = p[i].burst;
    }

    int front = 0, rear = 0, count = 0; /* circular ready queue */
    int completed = 0;
    int time = 0;

    while (completed < n) {
        /* Enqueue all processes that have arrived by 'time'. */
        for (int i = 0; i < n; i++) {
            if (!p[i].started && p[i].arrival <= time) {
                queue[rear] = i;
                rear = (rear + 1) % n;
                count++;
                p[i].started = 1;
            }
        }

        if (count == 0) {
            /* CPU idle: jump to the next arrival. */
            int next = -1;
            for (int i = 0; i < n; i++) {
                if (!p[i].started && (next == -1 || p[i].arrival < p[next].arrival))
                    next = i;
            }
            if (next == -1) break; /* nothing left to run */
            time = p[next].arrival;
            continue;
        }

        int idx = queue[front];
        front = (front + 1) % n;
        count--;

        int slice = p[idx].remaining < q ? p[idx].remaining : q;
        time += slice;
        p[idx].remaining -= slice;

        if (p[idx].remaining == 0) {
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        } else {
            /* Preempted: to the back of the queue. */
            queue[rear] = idx;
            rear = (rear + 1) % n;
            count++;
        }

        /* Pick up any process that arrived during the slice. */
        for (int i = 0; i < n; i++) {
            if (!p[i].started && p[i].arrival <= time) {
                queue[rear] = i;
                rear = (rear + 1) % n;
                count++;
                p[i].started = 1;
            }
        }
    }

    printf("%-5s %3s %3s %3s %4s %4s\n", "PID", "AT", "BT", "CT", "TAT", "WT");
    double sum_tat = 0.0, sum_wt = 0.0;
    for (int i = 0; i < n; i++) {
        printf("P%-4d %3d %3d %3d %4d %4d\n",
               p[i].id, p[i].arrival, p[i].burst, p[i].completion,
               p[i].turnaround, p[i].waiting);
        sum_tat += p[i].turnaround;
        sum_wt += p[i].waiting;
    }
    printf("Average TAT = %.2f, Average WT = %.2f\n",
           sum_tat / n, sum_wt / n);

    free(p);
    free(queue);
    return 0;
}
