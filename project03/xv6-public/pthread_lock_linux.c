#include <stdio.h>
#include <pthread.h>

int shared_resource = 0;

#define NUM_ITERS 1000000
#define NUM_THREADS 1000

typedef struct {
    int number[NUM_THREADS];
    int choosing[NUM_THREADS]; // true = 1, false = 0
} bakery_t;

bakery_t mutex;

void lock(bakery_t *mutex, int id);
void unlock(bakery_t *mutex, int id);

void init_lock(bakery_t *mutex) {
    for (int i = 0; i < NUM_THREADS; i++) {
        mutex->number[i] = 0;
        mutex->choosing[i] = 0; // false
    }
}

int max(int *arr, int sz) {
    int max = arr[0];
    for (int i = 1; i < sz; i++) { // i should start from 1
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

void lock(bakery_t *mutex, int id) {
    mutex->choosing[id] = 1; // true
    mutex->number[id] = max(mutex->number, NUM_THREADS) + 1;
    mutex->choosing[id] = 0; // false

    for (int i = 0; i < NUM_THREADS; i++) {
        while (mutex->choosing[i]); // wait until no thread is choosing
		//other thread has smaller number OR same number but smaller id -> must wait
        while (mutex->number[i] != 0 && 
               (mutex->number[i] < mutex->number[id] ||
               (mutex->number[i] == mutex->number[id] && i < id)));
    }
}

void unlock(bakery_t *mutex, int id) {
    mutex->number[id] = 0;
}

void* thread_func(void* arg) {
    int tid = *(int*)arg;

    lock(&mutex, tid);

    for (int i = 0; i < NUM_ITERS; i++) {
        shared_resource++;
    }

    unlock(&mutex, tid);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int tids[NUM_THREADS];

    init_lock(&mutex);

    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("shared: %d\n", shared_resource);

    return 0;
}

