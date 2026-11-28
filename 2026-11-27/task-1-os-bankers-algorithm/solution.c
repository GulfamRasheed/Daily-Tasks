#include <stdio.h>
#include <stdbool.h>

#define MAXP 32
#define MAXR 16

static int n, m;
static int alloc[MAXP][MAXR];
static int maxc[MAXP][MAXR];
static int need[MAXP][MAXR];
static int avail[MAXR];

static bool is_safe(int seq[], int *seq_len)
{
    int work[MAXR];
    bool finish[MAXP];
    int i, j, count = 0;
    bool found;

    for (j = 0; j < m; j++)
        work[j] = avail[j];
    for (i = 0; i < n; i++)
        finish[i] = false;

    while (count < n) {
        found = false;
        for (i = 0; i < n; i++) {
            bool ok = true;
            if (finish[i])
                continue;
            for (j = 0; j < m; j++) {
                if (need[i][j] > work[j]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                for (j = 0; j < m; j++)
                    work[j] += alloc[i][j];
                finish[i] = true;
                seq[count++] = i;
                found = true;
            }
        }
        if (!found)
            break;
    }
    *seq_len = count;
    return count == n;
}

static void print_seq(const int seq[], int len)
{
    int k;
    for (k = 0; k < len; k++)
        printf("P%d%s", seq[k] + 1, (k + 1 < len) ? " " : "\n");
}

int main(void)
{
    int i, j, request[MAXR];
    int seq[MAXP], seq_len = 0;

    if (scanf("%d %d", &n, &m) != 2 || n < 1 || n > MAXP || m < 1 || m > MAXR)
        return 1;

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            if (scanf("%d", &alloc[i][j]) != 1)
                return 1;

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            if (scanf("%d", &maxc[i][j]) != 1)
                return 1;

    for (j = 0; j < m; j++)
        if (scanf("%d", &avail[j]) != 1)
            return 1;

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = maxc[i][j] - alloc[i][j];

    printf("Need matrix:\n");
    for (i = 0; i < n; i++) {
        printf("P%d", i + 1);
        for (j = 0; j < m; j++)
            printf(" %d", need[i][j]);
        printf("\n");
    }

    if (is_safe(seq, &seq_len)) {
        printf("System is SAFE. Safe sequence: ");
        print_seq(seq, seq_len);
    } else {
        printf("System is UNSAFE: no safe sequence exists.\n");
        return 0;
    }

    for (j = 0; j < m; j++)
        if (scanf("%d", &request[j]) != 1)
            return 1;

    for (j = 0; j < m; j++) {
        if (request[j] != -1)
            break;
    }
    if (j == m) {
        printf("No request given; skipping request step.\n");
        return 0;
    }

    {
        int p = 1; /* 1-based process id receiving the request */
        int pid = p - 1;
        bool valid = true;

        printf("Request (");
        for (j = 0; j < m; j++)
            printf("%d%s", request[j], (j + 1 < m) ? " " : "");
        printf(") for P%d: ", p);

        if (p < 1 || p > n) {
            printf("REJECTED (invalid process id).\n");
            return 0;
        }
        for (j = 0; j < m; j++) {
            if (request[j] < 0 || request[j] > need[pid][j]) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            printf("REJECTED (exceeds declared need).\n");
            return 0;
        }
        for (j = 0; j < m; j++) {
            if (request[j] > avail[j]) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            printf("REJECTED (insufficient available resources).\n");
            return 0;
        }

        for (j = 0; j < m; j++) {
            avail[j] -= request[j];
            alloc[pid][j] += request[j];
            need[pid][j] -= request[j];
        }

        if (is_safe(seq, &seq_len)) {
            printf("GRANTED. New safe sequence: ");
            print_seq(seq, seq_len);
        } else {
            for (j = 0; j < m; j++) {
                avail[j] += request[j];
                alloc[pid][j] -= request[j];
                need[pid][j] += request[j];
            }
            printf("DENIED (would leave an unsafe state); rolled back.\n");
        }
    }

    return 0;
}
