#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5 // number of philosophers

// states
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];

sem_t mutex;
sem_t philosopher_sem[N];

// helper functions
int left(int i) { return (i + N - 1) % N; }
int right(int i) { return (i + 1) % N; }

// check whether philosopher 'i' can eat
void Test(int i) {
    if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) {
        state[i] = EATING;
        sem_post(&philosopher_sem[i]);
    }
}


void TakeForks(int i) {
    sem_wait(&mutex);

    state[i] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", i + 1);

    Test(i);

    sem_post(&mutex);

    // wait if forks not available
    sem_wait(&philosopher_sem[i]);
}

void PutForks(int i) {
    sem_wait(&mutex);

    state[i] = THINKING;
    printf("Philosopher %d put down forks and is THINKING\n", i + 1);

    // check whether neighbors can now eat
    Test(left(i));
    Test(right(i));

    sem_post(&mutex);
}

void* DiningPhilosophers(void* arg) {
    int id = *(int *) arg;
    while (1) {

        printf("Philosopher %d is THINKING\n", id + 1);
        sleep(1);

        TakeForks(id);

        printf("Philosopher %d is EATING\n", id + 1);
        sleep(1);

        PutForks(id);
    }
    return NULL;
}

int main() {
    pthread_t t[N];
    int id[N];
    
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++) {
        sem_init(&philosopher_sem[i], 0, 0);
        state[i] = THINKING;
        id[i] = i;
    }

    for (int i = 0; i < N; i++)  pthread_create(&t[i], NULL, DiningPhilosophers, &id[i]);
    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);

    sem_destroy(&mutex);
    for (int i = 0; i < N; i++) sem_destroy(&philosopher_sem[i]);

    return 0;
}