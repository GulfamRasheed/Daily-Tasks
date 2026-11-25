#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define SENTINEL (-1)

static int buffer[BUFFER_SIZE];
static int in_idx = 0;
static int out_idx = 0;

static sem_t empty;  /* free slots */
static sem_t full;   /* filled slots */
static sem_t mutex;  /* mutual exclusion */

static int next_item = 0;
static pthread_mutex_t item_lock = PTHREAD_MUTEX_INITIALIZER;

static void put(int item) {
    buffer[in_idx] = item;
    in_idx = (in_idx + 1) % BUFFER_SIZE;
}

static int get(void) {
    int item = buffer[out_idx];
    out_idx = (out_idx + 1) % BUFFER_SIZE;
    return item;
}

static void *producer(void *arg) {
    (void)arg;
    int items_per_producer = NUM_ITEMS / NUM_PRODUCERS;

    for (int i = 0; i < items_per_producer; i++) {
        pthread_mutex_lock(&item_lock);
        int item = next_item++;
        pthread_mutex_unlock(&item_lock);

        sem_wait(&empty);
        sem_wait(&mutex);
        put(item);
        printf("P: put %d\n", item);
        fflush(stdout);
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

static void *consumer(void *arg) {
    (void)arg;
    for (;;) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item = get();
        printf("C: got %d\n", item);
        fflush(stdout);
        sem_post(&mutex);
        sem_post(&empty);

        if (item == SENTINEL)
            break;
    }
    return NULL;
}

int main(void) {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_create(&consumers[i], NULL, consumer, NULL);
    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_create(&producers[i], NULL, producer, NULL);

    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    /* One sentinel per consumer so every consumer exits cleanly. */
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(SENTINEL);
        printf("P: put sentinel\n");
        fflush(stdout);
        sem_post(&mutex);
        sem_post(&full);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    printf("Done: all items produced and consumed.\n");

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    return 0;
}
