#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5 // number of philosophers
sem_t Fork[N]; // N Forks

/*
EXPLANATION:
Philosophers are either THINKING or EATING. 
A THINKING philosopher can EAT if neither of their neighbors are also EATING.
All odd numbered philosophers take the left forks first, then right.
All even numbered philosophers take their right forks first, then left.
[This is a little trick to avoid deadlock but it doesn't guarantee avoiding starvation]
[Also, it can be Even: left first then right, Odd: right first then left. They just can't be equal]
The philosopher is now EATING.
Puts the forks back down after they FINISHED EATING. 
*/
void* Philosopher(void* arg) {
    int id = *(int *) arg;

    int left = id;
    int right = (id + 1)%N;

    while (1) {
        printf("Philosopher %d is THINKING\n", id);
        sleep(1);

        if (id%2 == 0) { // even philosophers
            sem_wait(&Fork[left]);
            sem_wait(&Fork[right]);
        } else { // odd philosophers
            sem_wait(&Fork[right]);
            sem_wait(&Fork[left]);
        }

        printf("Philosopher %d is EATING\n", id);
        sleep(1);

        sem_post(&Fork[right]);
        sem_post(&Fork[left]);
        printf("Philosopher %d has FINISHED EATING\n", id);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int id[N];

    for (int i=0; i < N; i++) sem_init(&Fork[i], 0, 1);

    for (int i=0; i < N; i++) {
        id[i] = i+1;
        pthread_create(&philosophers[i], NULL, Philosopher, &id[i]);
    }

    for (int i=0; i < N; i++) pthread_join(philosophers[i], NULL);

    for (int i=0; i < N; i++) sem_destroy(&Fork[i]);
    return 0;
}



