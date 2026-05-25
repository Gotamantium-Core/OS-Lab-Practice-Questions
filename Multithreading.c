#include <stdio.h>
#include <math.h>
#include <pthread.h>

int data[100];
int n;

double mean, median, stddev;

void* Mean(void *) {
    int sum=0;
    for (int i=0; i < n; i++)
        sum += data[i];

    mean = sum / n;
    printf("Mean = %.2f\n", mean);
    return NULL;
}

void* Median(void *) {
    if (n%2)
        median = data[n/2];
    else
        median = (data[n/2] + data[(n-1)/2]) / 2;

    printf("Median = %.2f\n", median);
    return NULL;
}

// NOTE: this is population stddev, for sample stddev divide by n-1. 
void* StandardDeviation(void *) {
    int wSum=0;
    for (int i=0; i < n; i++)
        wSum += (data[i] - mean)*(data[i] - mean);
    
    stddev = sqrt(wSum/n);
    printf("Standard Deviation = %.2f\n", stddev);
    return NULL;
}

int main() {
    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter data: ");
    for (int i=0; i < n; i++)
        scanf("%d", &data[i]);
    ;

    // sorted inputs for median
    for (int i=0; i < n; i++)
        for (int j=i+1; j < n; j++) 
            if (data[j] < data[i]) {
                int t = data[i];
                data[i] = data[j];
                data[j] = t;
            }
    ;
    
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, Mean, NULL);
    pthread_create(&t2, NULL, Median, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_create(&t3, NULL, StandardDeviation, NULL);
    pthread_join(t3, NULL);
       
    return 0;
}