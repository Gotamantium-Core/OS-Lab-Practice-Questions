# Introduction
This repo contains a bunch of programs collected from the syllabus of **PCCSL407: Operating Systems Lab**.

## Programs 

### Address translation 

The virtual address size, page size, and decimal address are taken as command line arguments and converted into physical address in the form of `<frame, offset>`.

### Banker's Algorithm

A basic version of Banker's computes the need matrix from allocation, and max matrix and determines whether a system is deadlocked or not. 

### CPU Scheduling Algorithms

Obtains the arrival times, burst times, and priorities (and time quantum for round robin) and scheddules them accordingly 

The program features the following algorithms

1. First Come First Serve (FCFS)
2. Shortest Job First (SJF) _[non preemptive]_
3. Shortest Remaining Time First (SRTF)
4. Preemptive and Non-preemptive Priority _[lower value = higher priority]_
5. Round Robin (RR)

### Dining Philosophers

Two solutions for the same problem. The first one involves making odd-numbered philosophers taking their right then left and even-numbered philosophers taking their left then right. 

The second solution is state based with: `THINKING`, `HUNGRY`, and `EATING`. 

Both solutions guarantee deadlock avoidance but aren't necessarily starvation-free. 

### Disk Scheduling Algorithms

The initial head position is taken as a command line argument and the requests are randomly generated within the specified range and services the requests accordingly. 

The program features the following algorithms

1. First Come First Serve (FCFS)
2. Shortest Seek Time First (SSTF)
3. Elevator (SCAN)
4. Circular SCAN (CSCAN)
5. LOOK 
6. Circular LOOK (CLOOK)

### Multithreading

A simple multithreading program that involves taking a dataset as input from the user and using separate threads to calculate: mean, median, and standard deviation of the given dataset. 

In the program, I use `pthread.h` which needs the `-lpthread` flag (as well as the `-lm` flag for `math.h`) when compiling. 

### Page Replacement Algorithms

Takes the number of frames and the length of the reference string as command line arguments and randomly generates a sequence of requests and performs replacement on them accordingly.

The program has the following replacement algorithms

1. First In First Out (FIFO)
2. Least Recently Used (LRU)
3. Least Frequently Used (LFU)
4. Optimal (OPT)

### Readers Writers

This program features a solution to the readers writers problem with writer priority. 
