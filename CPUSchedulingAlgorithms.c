#include <stdio.h>
#include <limits.h>

/*
Takes the arrival and burst times (and priorities) of n processes and schedules them based on 5 algorithms

CONTAINS: {
    First Come First Serve (FCFS), 
    Shortest Job First (SJF), 
    Shortest Remaining Time First (SRTF), 
    Preemptive and Non-preemptive priority (LOWER VALUE = HIGHER PRIORITY), 
    Round Robin (RR)
}

Each function returns the average waiting time (in double) and also constructs a Gantt chart based on the scheduling algorithm
(**NOTE**): FCFS, SJF are both non-preemptive
*/

typedef struct Process {
    int pid;
    int at, bt;
    int pr;
} Process;
typedef struct Block { // for Gantt chart creation
    int start, stop;
    int pid;
} Block;

// helper functions
void CopyProcess(Process dest[], Process src[], int n) {
    for (int i=0; i < n; i++) dest[i]=src[i];
}
void DisplayGanttChart(Block g[], int n) {
    printf("Gantt chart: ");
    for (int i=0; i < n; i++)
        printf("| P%d(%d-%d) ", g[i].pid, g[i].start, g[i].stop);
    printf("| \n");
}
void InputProcess(Process p[], int n) {
    printf("(PID will be assigned automatically)\n");

    printf("Input processes (AT, BT, PR)\n");

    for (int i=0; i < n; i++) {
        p[i].pid = i+1;

        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
    }
}

// algorithms

double fcfs(Process p[], int n, Block g[], int* k) {
    int time=0, wt=0;
    *k=0;

    // sort by arrival time
    for (int i=0; i < n; i++)
        for (int j=i+1; j < n; j++)
            if (p[j].at < p[i].at || (p[i].at == p[j].at && p[j].pid < p[i].pid)) {
                Process t = p[i];
                p[i] = p[j];
                p[j] = t;
            }
    ;
    for (int i=0; i < n; i++) {
        if (p[i].at > time) time = p[i].at;
        
        g[*k].pid = p[i].pid;
        g[*k].start = time;
        g[*k].stop = time + p[i].bt;
        (*k)++;

        wt += time - p[i].at;
        time += p[i].bt;
    }
    return (double) wt / n;
}

double sjf(Process p[], int n, Block g[], int* k) {
    int time=0, wt=0, completed=0;

    int visited[n];
    for (int i=0; i < n; i++) visited[i]=0;
    *k = 0;

    while (completed < n) {
        int idx=-1, minBT=INT_MAX;
        for (int i=0; i < n; i++)
            if (!visited[i] && p[i].at <= time && p[i].bt < minBT) {
                minBT = p[i].bt;
                idx = i;
            }
        ;
        if (idx == -1) {
            time++;
            continue;
        }

        g[*k].pid = p[idx].pid;
        g[*k].start = time;
        g[*k].stop = time + p[idx].bt;
        (*k)++;

        wt += time - p[idx].at;
        time += p[idx].bt;

        visited[idx]=1;
        completed++;
    }
    return (double) wt / n;
}

double srtf(Process p[], int n, Block g[], int* k) {
    int time=0, wt=0, completed=0, last=-1;
    
    int rt[n];
    for (int i=0; i < n; i++) rt[i] = p[i].bt;

    *k = 0;

    while (completed < n) {
        int idx=-1, minRT=INT_MAX;
        for (int i=0; i < n; i++)
            if (p[i].at <= time && rt[i] > 0 && rt[i] < minRT) {
                minRT = rt[i];
                idx = i;
            }
        ;
        if (idx == -1) {
            time++;
            continue;
        }

        if (last != idx) {
            if (*k > 0) g[*k - 1].stop = time;

            g[*k].pid = p[idx].pid;
            g[*k].start = time;
            (*k)++;

            last = idx;
        }

        rt[idx]--;
        time++;

        if (rt[idx] == 0) {
            completed++;
            wt += time - p[idx].at - p[idx].bt; 
        }
    }
    g[*k-1].stop = time;
    return (double) wt / n;
}

// lower num = higher priority
double nonpreemptive_priority(Process p[], int n, Block g[], int* k) {
    int time=0, wt=0, completed=0;
    
    int done[n];
    for (int i=0; i < n; i++) done[i] = 0;

    *k = 0;

    while (completed < n) {
        int idx=-1, bestP=INT_MAX;
        for (int i=0; i < n; i++)
            if (!done[i] && p[i].at <= time && p[i].pr < bestP) {
                bestP = p[i].pr;
                idx = i;
            }
        ;
        if (idx == -1) {
            time++;
            continue;
        }

        g[*k].pid = p[idx].pid;
        g[*k].start = time;
        g[*k].stop = time + p[idx].bt;
        (*k)++;

        wt += time - p[idx].at;
        time += p[idx].bt;

        completed++;
        done[idx]=1;
    }
    return (double) wt / n;
}

// lower num = higher priority
double preemptive_priority(Process p[], int n, Block g[], int* k) {
    int time=0, wt=0, completed=0, last=-1;

    int rt[n];
    for (int i=0; i < n; i++) rt[i] = p[i].bt;

    *k = 0;

    while (completed < n) {
        int idx=-1, bestP=INT_MAX;
        for (int i=0; i < n; i++) 
            if (p[i].at <= time && rt[i] > 0 && p[i].pr < bestP) {
                bestP = p[i].pr;
                idx = i;
            }
        ;
        if (idx == -1) {
            time++;
            continue;
        }

        if (last != idx) {
            if (*k > 0) g[*k - 1].stop = time;

            g[*k].pid = p[idx].pid;
            g[*k].start = time;

            (*k)++;
            last = idx;
        }

        rt[idx]--;
        time++;
        
        if (rt[idx] == 0) {
            completed++;
            wt += time - p[idx].at - p[idx].bt;
        }
    }
    g[*k-1].stop = time;
    return (double) wt / n;
}

double rr(Process p[], int n, int q, Block g[], int* k) {
    int time=0, wt=0, completed=0;

    int bt[n];
    for (int i=0; i < n; i++) bt[i] = p[i].bt;

    *k = 0;

    while (completed < n){
        int moved=0;
        for (int i=0; i < n; i++) {
            if (bt[i] > 0 && p[i].at <= time) {
                moved=1;

                int start = time;
                int used = (bt[i] > q) ? q : bt[i];

                bt[i] -= used;
                time += used;

                g[*k].pid = p[i].pid;
                g[*k].start = start;
                g[*k].stop = time;
                (*k)++;

                if (bt[i] == 0) {
                    completed++;
                    wt += time - p[i].at - p[i].bt;
                }
            }
        }
        if (!moved) time++;
    }
    return (double) wt / n;
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n], temp[n];
    InputProcess(p, n);

    int q;
    printf("Enter time quantum for round robin: ");
    scanf("%d", &q);


    // testing area
    CopyProcess(temp, p, n);
    Block g_fcfs[100], g_sjf[100], g_srtf[100], g_np_pr[100], g_p_pr[100], g_rr[100];
    int k_fcfs, k_sjf, k_srtf, k_np_pr, k_p_pr, k_rr;
    
    double avg_fcfs = fcfs(temp, n, g_fcfs, &k_fcfs);
    CopyProcess(temp, p, n); // Realistically should be done in between every algorithm
    // but here, only FCFS directly modifies the input data, so just here is sufficient.

    double avg_sjf = sjf(temp, n, g_sjf, &k_sjf);
    double avg_srtf = srtf(temp, n, g_srtf, &k_srtf);
    double avg_np_pr = nonpreemptive_priority(temp, n, g_np_pr, &k_np_pr);
    double avg_p_pr = preemptive_priority(temp, n, g_p_pr, &k_p_pr);
    double avg_rr = rr(temp, n, q, g_rr, &k_rr);


    // output area
    printf("Average waiting time for: FCFS = %.2f\n", avg_fcfs);
    DisplayGanttChart(g_fcfs, k_fcfs);
    printf("Average waiting time for: SJF = %.2f\n", avg_sjf);
    DisplayGanttChart(g_sjf, k_sjf);
    printf("Average waiting time for: SRTF = %.2f\n", avg_srtf);
    DisplayGanttChart(g_srtf, k_srtf);
    printf("Average waiting time for: Non-preemptive Priority = %.2f\n", avg_np_pr);
    DisplayGanttChart(g_np_pr, k_np_pr);
    printf("Average waiting time for: Preemptive Priority = %.2f\n", avg_p_pr);
    DisplayGanttChart(g_p_pr, k_p_pr);
    printf("Average waiting time for: Round Robin(q=%d) = %.2f\n", q, avg_rr);
    DisplayGanttChart(g_rr, k_rr);

    return 0;
}