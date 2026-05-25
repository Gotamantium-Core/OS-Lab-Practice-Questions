#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

/*
Generates MAX_CYLINDERS number of random requests between (0,MAX-1) and services them based on 6 algorithms

CONTAINS: {
    First Come First Serve (FCFS),
    Shortest Seek Time First (SSTF),
    Elevator (SCAN),
    Circular SCAN (CSCAN),
    LOOK,
    Circular LOOK (CLOOK)
}

Each function returns the total head movement by the algorithm
For SCAN, CSCAN, LOOK, and CLOOK: the convention (here) is {
    direction = 0 => right first then left
    direction = 1 => left first then right
}
(**NOTE**): For clustered datasets, SSTF behaves similar to LOOK
*/

#define MAX 5000
#define MAX_CYLINDERS 10
// helper functions

// returns the distance between two points a and b
int distance(int a, int b) {
    return abs(a-b);
}
void sortArray(int arr[], int n) {
    for (int i=1; i < n; i++) {
        int j=i-1;
        int key = arr[i];

        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}
int minIndex(int arr[], int n, int visited[]) {
    int m = -1;
    for (int i=0; i < n; i++) if (!visited[i] && (m == -1 || arr[m] > arr[i])) m = i;
    return m;
}
void reloadDistances(int req[], int dist[], int n, int pos, int visited[]) {
    for (int i=0; i < n; i++)
        dist[i] = (visited[i]) ? INT_MAX : distance(req[i], pos);
}

// algorithms

int fcfs(int requests[MAX_CYLINDERS], int n, int head) {
    printf("\nFCFS:\n");
    int movement=0;
    for (int i=0; i < n; i++) {
        printf("MOVE FROM %d -> %d\n", head, requests[i]);
        movement += distance(head, requests[i]);
        head = requests[i];
    }
    return movement;
}


int sstf(int requests[MAX_CYLINDERS], int n, int head) {
    printf("\nSSTF:\n");
    int movement=0, completed=0;
    int distances[n], visited[n];

    for (int i=0; i < n; i++) visited[i] = 0;

    while (completed < n) {
        reloadDistances(requests, distances, n, head, visited);
        int next = minIndex(distances, n, visited);

        printf("MOVE FROM %d -> %d\n", head, requests[next]);

        movement += distances[next];
        head = requests[next];

        visited[next] = 1;
        completed++;
    }
    return movement;
}

int scan(int requests[MAX_CYLINDERS], int n, int head, int direction) {
    printf("\nSCAN:\n");
    int movement=0, index=0;

    sortArray(requests, n);
    while (index < n && requests[index] < head) index++; // find head's place

    if (direction == 0) { // Right first
        for (int i=index; i < n; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        printf("MOVE FROM %d -> %d\n", head, MAX-1);
        movement += distance(head, MAX-1);
        head = MAX-1;
        for (int i=index-1; i >= 0; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    } else { // Left first
        for (int i=index-1; i >= 0; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        printf("MOVE FROM %d -> 0\n", head);
        movement += distance(head, 0);
        head = 0;
        for (int i=index; i < n; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    }
    return movement;
} 

int cscan(int requests[MAX_CYLINDERS], int n, int head, int direction) {
    printf("\nC-SCAN:\n");
    int movement=0, index=0;
    sortArray(requests, n);

    while (index < n && requests[index] < head) index++;

    if (direction == 0) {
        for (int i=index; i < n; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        printf("MOVE FROM %d -> %d\n", head, MAX-1);
        movement += distance(head, MAX-1);
        head = MAX-1;

        printf("MOVE FROM %d -> %d\n", head, 0);
        movement += distance(head, 0);
        head = 0;
        for (int i=0; i < index; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    } else {
        for (int i=index-1; i >= 0; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        printf("MOVE FROM %d -> %d\n", head, 0);
        movement += distance(head, 0);
        head = 0;

        printf("MOVE FROM %d -> %d\n", head, MAX-1);
        movement += distance(head, MAX-1);
        head = MAX-1;
        for (int i=n-1; i >= index; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    }
    return movement;
}

int look(int requests[MAX_CYLINDERS], int n, int head, int direction) {
    printf("\nLOOK:\n");
    int movement=0, index=0;

    sortArray(requests, n);
    while (index < n && requests[index] < head) index++; // find head's place

    if (direction == 0) { // Right first
        for (int i=index; i < n; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        for (int i=index-1; i >= 0; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    } else { // Left first
        for (int i=index-1; i >= 0; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        for (int i=index; i < n; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    }
    return movement;
} 

int clook(int requests[MAX_CYLINDERS], int n, int head, int direction) {
    printf("\nC-LOOK:\n");
    int movement=0, index=0;
    sortArray(requests, n);

    while (index < n && requests[index] < head) index++;

    if (direction == 0) {
        for (int i=index; i < n; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        for (int i=0; i < index; i++) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    } else {
        for (int i=index-1; i >= 0; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
        for (int i=n-1; i >= index; i--) {
            printf("MOVE FROM %d -> %d\n", head, requests[i]);

            movement += distance(head, requests[i]);
            head=requests[i];
        }
    }
    return movement;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <Initial Position of Disk Head>\n", argv[0]);
        printf("\tNOTE: disk head must be in between 1-4999 (both inclusive)\n");
        return 1;
    }

    int head = atoi(argv[1]);
    if (head < 0 || head >= MAX) {
        printf("Invalid head position\n");
        return 1;
    }
    
    srand(time(NULL)); // to avoid rand() returning same values

    int requests[MAX_CYLINDERS];
    for (int i=0; i < MAX_CYLINDERS; i++) requests[i] = rand() % MAX; // generate 10 random values between 0-4999
    

    printf("Generated requests string: ");
    for (int i=0; i < MAX_CYLINDERS; i++) printf("%d ", requests[i]);
    printf("\n");

    printf("Initial head position: %d\n", head);

    int move_fcfs = fcfs(requests, MAX_CYLINDERS, head);
    int move_sstf = sstf(requests, MAX_CYLINDERS, head);
    int move_scan = scan(requests, MAX_CYLINDERS, head, 0);
    int move_look = look(requests, MAX_CYLINDERS, head, 0);
    int move_cscan = cscan(requests, MAX_CYLINDERS, head, 0);
    int move_clook = clook(requests, MAX_CYLINDERS, head, 0);

    printf("\n\nTotal head movements: \n");
    printf("FCFS = %d\nSSTF = %d\nSCAN=%d\nCSCAN = %d\nLOOK = %d\nCLOOK = %d\n", move_fcfs, move_sstf, move_scan, move_cscan, move_look, move_clook);
    return 0;
}