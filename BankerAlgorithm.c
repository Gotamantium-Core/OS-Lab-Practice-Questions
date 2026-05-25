#include <stdio.h>

#define MAX 100

// Returns 1 when a deadlock occurs and prints safe sequence and returns 0 if it doesn't 
int Banker(int allocation[MAX][MAX], int max[MAX][MAX], int available[MAX], int p, int r) {
    int completed=0;
    int safeseq[p];

    int need[MAX][MAX];
    for (int i=0; i < p; i++)
        for (int j=0; j < r; j++)
            need[i][j] = max[i][j] - allocation[i][j];
    ;
    int finished[p];
    for (int i=0; i < p; i++) finished[i]=0;

    while (completed < p) {
        int found=0;
        for (int i=0; i < p; i++) {
            if (!finished[i]) {
                int possible=1;
                for (int j=0; j < r; j++)
                    if (need[i][j] > available[j]) {
                        possible=0; // this process is not doable. Skip it
                        break;
                    }
                if (possible) {
                    for (int k=0; k < r; k++) available[k] += allocation[i][k]; // update available vector

                    safeseq[completed++] = i;
                    finished[i] = 1;
                    found=1;
                }
            }
        }
        if (!found) { // no processes could run with the current resources available => deadlock
            printf("Deadlock detected\n");
            return 1;
        }
    }
    printf("System is in a safe state. \nSafe sequence: ");
    for (int i=0; i < p; i++) {
        printf("P%d", safeseq[i]);
        if (i != p-1) printf(" -> ");
    }
    printf("\n");
    return 0;
}



int main() {
    int p, r;
    printf("Enter number of processes and resources: ");
    scanf("%d %d", &p, &r);

    int allocation[MAX][MAX], max[MAX][MAX], available[r];

    printf("Enter allocation matrix: \n");
    for (int i=0; i < p; i++)
        for (int j=0; j < r; j++)
            scanf("%d", &allocation[i][j]);
    ;
    printf("Enter max matrix: \n");
    for (int i=0; i < p; i++)
        for (int j=0; j < r; j++)
            scanf("%d", &max[i][j]);
    ;
    printf("Enter available vector: ");
    for (int i=0; i < r; i++)
            scanf("%d", &available[i]);
    ;

    Banker(allocation, max, available, p, r);
    return 0;
}
