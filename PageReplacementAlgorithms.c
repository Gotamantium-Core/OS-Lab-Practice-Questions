#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX 100
#define MAX_PAGES 20

int find(int arr[], int n, int target) {
    for (int i = 0; i < n; i++)
        if (arr[i] == target)
            return i;
    return -1;
}

int min(int arr[], int n) {
    int m = 0;
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[m]) m = i;
    return m;
}

int max(int arr[], int n) {
    int m = 0;
    for (int i = 1; i < n; i++)
        if (arr[i] > arr[m]) m = i;
    return m;
}

void displayMatrix(int arr[MAX][MAX_PAGES], int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (arr[i][j] == -1) printf("- ");
            else printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int distance(int arr[], int n, int start, int target) {
    for (int i = start; i < n; i++)
        if (arr[i] == target)
            return i - start;
    return INT_MAX;
}

void addRowToMatrix(int arr[MAX][MAX_PAGES], int row[MAX_PAGES], int rowSize, int rowVal) {
    for (int i = 0; i < rowSize; i++)
        arr[rowVal][i] = row[i];
}

/* ---------------- FIFO (OK) ---------------- */
int fifo(int ref[], int n, int f, int out[MAX][MAX_PAGES]) {
    int frames[MAX_PAGES], k = 0, faults = 0;

    for (int i = 0; i < f; i++) frames[i] = -1;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < f; j++)
            out[i][j] = -1;

    for (int i = 0; i < n; i++) {
        if (find(frames, f, ref[i]) == -1) {
            frames[k] = ref[i];
            k = (k + 1) % f;
            faults++;
        }
        addRowToMatrix(out, frames, f, i);
    }
    return faults;
}

/* ---------------- LRU (FIXED) ---------------- */
int lru(int ref[], int n, int f, int out[MAX][MAX_PAGES]) {
    int frames[MAX_PAGES], time[MAX_PAGES];
    int faults = 0, clock = 0;

    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        time[i] = -1;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < f; j++)
            out[i][j] = -1;

    for (int i = 0; i < n; i++) {
        clock++;

        int idx = find(frames, f, ref[i]);

        if (idx == -1) {
            int replace = find(frames, f, -1);
            if (replace == -1) {
                replace = min(time, f);
            }
            idx = replace;
            frames[idx] = ref[i];
            faults++;
        }

        time[idx] = clock;
        addRowToMatrix(out, frames, f, i);
    }
    return faults;
}

/* ---------------- LFU (FIXED) ---------------- */
int lfu(int ref[], int n, int f, int out[MAX][MAX_PAGES]) {
    int frames[MAX_PAGES], freq[MAX_PAGES];
    int faults = 0;

    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        freq[i] = 0;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < f; j++)
            out[i][j] = -1;

    for (int i = 0; i < n; i++) {
        int idx = find(frames, f, ref[i]);

        if (idx == -1) {
            int replace = find(frames, f, -1);

            if (replace == -1) {
                int minIdx = 0;
                for (int j = 1; j < f; j++) {
                    if (freq[j] < freq[minIdx])
                        minIdx = j;
                }
                replace = minIdx;
            }

            idx = replace;
            frames[idx] = ref[i];
            freq[idx] = 1;
            faults++;
        } else {
            freq[idx]++;
        }

        addRowToMatrix(out, frames, f, i);
    }
    return faults;
}

/* ---------------- OPT (FIXED minimal) ---------------- */
int opt(int ref[], int n, int f, int out[MAX][MAX_PAGES]) {
    int frames[MAX_PAGES], dist[MAX_PAGES];
    int faults = 0;

    for (int i = 0; i < f; i++) frames[i] = -1;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < f; j++)
            out[i][j] = -1;

    for (int i = 0; i < n; i++) {
        int idx = find(frames, f, ref[i]);

        if (idx == -1) {
            int replace = find(frames, f, -1);

            if (replace == -1) {
                for (int j = 0; j < f; j++) {
                    dist[j] = distance(ref, n, i + 1, frames[j]);
                }
                replace = max(dist, f);
            }

            frames[replace] = ref[i];
            faults++;
        }

        addRowToMatrix(out, frames, f, i);
    }

    return faults;
}

/* ---------------- MAIN ---------------- */
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <length> <frames>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int f = atoi(argv[2]);

    if (n <= 0 || n > MAX_PAGES || f <= 0 || f > 7) {
        printf("Invalid input\n");
        return 1;
    }

    srand(time(NULL));

    int ref[MAX_PAGES];
    printf("Reference string: ");
    for (int i = 0; i < n; i++) {
        ref[i] = rand() % 10;
        printf("%d ", ref[i]);
    }
    printf("\n\n");

    int out[MAX][MAX_PAGES];

    printf("FIFO Faults: %d\n", fifo(ref, n, f, out));
    displayMatrix(out, n, f);

    printf("LRU Faults: %d\n", lru(ref, n, f, out));
    displayMatrix(out, n, f);

    printf("LFU Faults: %d\n", lfu(ref, n, f, out));
    displayMatrix(out, n, f);

    printf("OPT Faults: %d\n", opt(ref, n, f, out));
    displayMatrix(out, n, f);

    return 0;
}