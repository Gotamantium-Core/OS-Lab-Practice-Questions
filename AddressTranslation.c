#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s <Virtual Address Size (MB)> <Page size (KB)> <Virtual Address (dec)>\n", argv[0]);
        return 1;
    }

    // convert everything to bytes
    long virtualAddressSize = atoi(argv[1]) * 1024 * 1024;
    long pageSize = atoi(argv[2]) * 1024;

    // virtual address in bytes
    int virtualAddress = atoi(argv[3]);

    int numPages = virtualAddressSize / pageSize;

    // create the page table with sample values (frame number = 2*page number)
    int pageTable[numPages];
    int valid[numPages]; // 1 if loaded in memory, 0 if not
    for (int i=0; i < numPages; i++) {
        pageTable[i] = 2*i; // This function can be anything else
        valid[i] = 1; // all pages are in memory
    }

    long pageNumber = virtualAddress / pageSize;
    long offset = virtualAddress % pageSize;

    if (pageNumber < 0 || pageNumber >= numPages) {
        printf("Invalid address\n");
        return 1;
    }

    long frameNumber = 0;

    // Won't happen here but you get the point. 
    if (!valid[pageNumber]) {
        printf("Page table MISS!\nLoading into memory...\n");

        pageTable[pageNumber]=2*pageNumber; // map according to the function from page table initialization
        valid[pageNumber] = 1;

        frameNumber = pageTable[pageNumber];
    } else {
        printf("Page table HIT!\nFetching value...\n");
        frameNumber = pageTable[pageNumber];
    }

    printf("Physical address: \n");
    printf("In decimal: %ld \n<frame, offset>: ", (frameNumber * pageSize) + offset);
    printf("<%ld, %ld>\n", frameNumber, offset);
    return 0;
}