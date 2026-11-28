/*
 * Page-reference-string simulator: FIFO, LRU and Optimal replacement.
 *
 * Input (stdin):
 *   n_frames n_refs
 *   ref_1 ref_2 ... ref_n_refs   (non-negative page numbers, may span lines)
 *
 * Output: per-reference trace (hit/fault + frame contents) and the total
 * page-fault count for each of the three policies.
 *
 * Build: gcc -std=c11 -O2 -Wall -Wextra -o page_repl solution.c
 * Run:   page_repl < input.txt
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 32
#define MAX_REFS   4096

static void show(int step, int page, int hit, const int *frames, int nf)
{
    printf("  %3d: page %2d -> %-4s frames [", step, page, hit ? "hit" : "fault");
    for (int i = 0; i < nf; i++) {
        putchar(' ');
        if (frames[i] < 0)
            printf(" .");
        else
            printf("%2d", frames[i]);
    }
    printf(" ]\n");
}

/* FIFO: frames are filled in slot order; once full, frames[next] is oldest. */
static int run_fifo(const int *refs, int n, int nf)
{
    int frames[MAX_FRAMES];
    int size = 0, next = 0, faults = 0;

    for (int i = 0; i < nf; i++)
        frames[i] = -1;

    printf("FIFO:\n");
    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int hit = 0;
        for (int j = 0; j < nf; j++)
            if (frames[j] == page) {
                hit = 1;
                break;
            }

        if (!hit) {
            faults++;
            if (size < nf) {
                frames[size++] = page;
            } else {
                frames[next] = page;
                next = (next + 1) % nf;
            }
        }
        show(i + 1, page, hit, frames, nf);
    }
    printf("FIFO total page faults: %d\n\n", faults);
    return faults;
}

/* LRU: each frame records the index of its last use; evict the smallest. */
static int run_lru(const int *refs, int n, int nf)
{
    int frames[MAX_FRAMES], used[MAX_FRAMES];
    int size = 0, faults = 0;

    for (int i = 0; i < nf; i++) {
        frames[i] = -1;
        used[i] = -1;
    }

    printf("LRU:\n");
    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int hit_at = -1;
        for (int j = 0; j < nf; j++)
            if (frames[j] == page) {
                hit_at = j;
                break;
            }

        if (hit_at >= 0) {
            used[hit_at] = i;
        } else {
            faults++;
            if (size < nf) {
                frames[size] = page;
                used[size] = i;
                size++;
            } else {
                int victim = 0;
                for (int j = 1; j < nf; j++)
                    if (used[j] < used[victim])
                        victim = j;
                frames[victim] = page;
                used[victim] = i;
            }
        }
        show(i + 1, page, hit_at >= 0, frames, nf);
    }
    printf("LRU total page faults: %d\n\n", faults);
    return faults;
}

/* Optimal: evict the page whose next use is farthest in the future. */
static int run_opt(const int *refs, int n, int nf)
{
    int frames[MAX_FRAMES];
    int size = 0, faults = 0;

    for (int i = 0; i < nf; i++)
        frames[i] = -1;

    printf("Optimal:\n");
    for (int i = 0; i < n; i++) {
        int page = refs[i];
        int hit_at = -1;
        for (int j = 0; j < nf; j++)
            if (frames[j] == page) {
                hit_at = j;
                break;
            }

        if (hit_at < 0) {
            faults++;
            if (size < nf) {
                frames[size++] = page;
            } else {
                int victim = 0, farthest = -1;
                for (int j = 0; j < nf; j++) {
                    int next_use = n; /* never referenced again */
                    for (int k = i + 1; k < n; k++)
                        if (refs[k] == frames[j]) {
                            next_use = k;
                            break;
                        }
                    if (next_use > farthest) {
                        farthest = next_use;
                        victim = j;
                    }
                }
                frames[victim] = page;
            }
        }
        show(i + 1, page, hit_at >= 0, frames, nf);
    }
    printf("Optimal total page faults: %d\n\n", faults);
    return faults;
}

int main(void)
{
    int nf, n = 0;
    int refs[MAX_REFS];

    if (scanf("%d %d", &nf, &n) != 2 ||
        nf <= 0 || nf > MAX_FRAMES || n <= 0 || n > MAX_REFS) {
        fprintf(stderr, "usage: n_frames n_refs followed by the reference string\n");
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &refs[i]) != 1) {
            fprintf(stderr, "error: expected %d page numbers\n", n);
            return 1;
        }
        if (refs[i] < 0) {
            fprintf(stderr, "error: page numbers must be non-negative\n");
            return 1;
        }
    }

    run_fifo(refs, n, nf);
    run_lru(refs, n, nf);
    run_opt(refs, n, nf);
    return 0;
}
