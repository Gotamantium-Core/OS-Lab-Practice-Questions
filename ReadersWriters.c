#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int readCount=0, writeCount=0;


/*
MUTEX protects shared counters like readCount and writeCount from race conditions.
RESOURCE provides exclusive access to the critical section, allowing either multiple readers or one writer.
QUEUE acts as a fairness gate that prevents new readers from entering when a writer is waiting.
*/
sem_t mutex;   
sem_t resource; 
sem_t queue; 

/*
EXPLANATION:
Step 1: Reader enters the system through the fairness gate using sem_wait(&queue). If a writer is waiting, new readers are blocked.
Step 2: Reader safely increments read_count using sem_wait(&mutex) and sem_post(&mutex).
Step 3: If it is the first reader, it locks the shared resource using sem_wait(&resource), preventing writers from entering the critical section.
Step 4: Reader performs reading.
Step 5: Reader safely decrements read_count using sem_wait(&mutex) and sem_post(&mutex).
Step 6: If it is the last reader, it releases the shared resource using sem_post(&resource), allowing writers to proceed.
*/
void* reader(void* arg) {
    int id = *(int *) arg;

    sem_wait(&queue);

    sem_wait(&mutex);
    readCount++;
    if (readCount == 1)
        sem_wait(&resource); // first reader locks the resources
    sem_post(&mutex);

    sem_post(&queue);

    printf("Reader %d is READING\n", id);

    sem_wait(&mutex);
    readCount--;
    if (readCount == 0)
        sem_post(&resource); // last reader unlocks the resources 
    sem_post(&mutex);

    return NULL;
}

/*
EXPLANATION:
Step 1: Writer arrives.
Step 2: Writer safely increments write_count using sem_wait(&mutex) and sem_post(&mutex).
Step 3: If it is the first writer, it blocks new readers from entering the system using sem_wait(&queue). Existing readers are allowed to finish.
Step 4: Writer enters the critical section exclusively using sem_wait(&resource).
Step 5: Writer performs writing.
Step 6: Writer exits the critical section using sem_post(&resource).
Step 7: Writer safely decrements write_count using sem_wait(&mutex) and sem_post(&mutex).
Step 8: If it is the last writer, it releases the fairness gate using sem_post(&queue), allowing readers to enter again.
*/
void* writer(void* arg) {
    int id = *(int *) arg;

    sem_wait(&mutex);
    writeCount++;
    if (writeCount == 1)
        sem_wait(&queue); // block new readers
    sem_post(&mutex);

    sem_wait(&resource);
    printf("Writer %d is WRITING\n", id);
    sleep(1);
    sem_post(&resource);

    
    sem_wait(&mutex);
    writeCount--;
    if (writeCount == 0)
        sem_post(&queue); // last writer allows readers again
    sem_post(&mutex);

    return NULL;
}

int main() {
    int r, w;
    printf("Enter number of readers and writers: ");
    scanf("%d %d", &r, &w);

    int rids[r], wids[w];
    for (int i=0; i < r; i++) rids[i] = i+1;
    for (int i=0; i < w; i++) wids[i] = i+1;

    sem_init(&resource, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&queue, 0, 1);


    pthread_t Reader[r], Writer[w];
    for (int i=0; i < r; i++) pthread_create(&Reader[i], NULL, reader, &rids[i]);
    for (int i=0; i < w; i++) pthread_create(&Writer[i], NULL, writer, &wids[i]);

    for (int i=0; i < r; i++) pthread_join(Reader[i], NULL);
    for (int i=0; i < w; i++) pthread_join(Writer[i], NULL);

    // destroy semaphores after use
    sem_destroy(&mutex);
    sem_destroy(&resource);
    sem_destroy(&queue);
    return 0;
}


