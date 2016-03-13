/* 
 *  allocate memory for K input buffers and 1 output buffers
 *  merge buffer arrays into output buffers and add into heap
 * */

#include "Merge.h"

/* initialize MergeManager */
int initMerger(char *inputFile, char *outputFile, int chunkNumber) {
    MergeManager *merger = (MergeManager *)malloc(sizeof(MergeManager));

    FILE *inputFP, *outputFP;
    if (!(inputFP = fopen(inputFile, "rb"))) {
        fprintf(stderr, "Couldn't read file \"%s\"\n", inputFile);
        exit(0);
    }

    if (!(outputFP = fopen(outputFile, "wb"))) {
        fprintf(stderr, "Couldn't write to file \"%s\"\n", outputFile);
        exit(0);
    }


    /* get inputFile size */
    fseek(inputFP, 0, SEEK_END);
    int fileSize = ftell(inputFP);
    fseek(inputFP, 0, SEEK_SET);

    /* get chunk size and chunk elements number */
    int chunkSize = fileSize / chunkNumber;
    int remainder = chunkSize % sizeof(Record);
    int lastChunkSize = chunkSize;
    if (remainder != 0) {
        chunkSize = chunkSize - remainder + sizeof(Record);
        lastChunkSize = chunkSize - (sizeof(Record) - remainder) * (chunkNumber - 1);
    }
    int elementsPerChunk = chunkSize / sizeof(Record);

    /* initialize buffer elements number */
    int elementsPerBuffer = 10;
    
    /* initialize output buffer */
    Record *outputBuffer = (Record *)calloc(elementsPerBuffer, sizeof(Record));

    /* initialize input buffers */
    InputBuffer *inputBuffers = (InputBuffer *)calloc(chunkNumber, sizeof(InputBuffer));
    /* initialize run length */
    int i;
    for (i = 0; i < chunkNumber-1; i++) {
        (inputBuffers + i)->runLength = chunkSize;
    }
    (inputBuffers + chunkNumber - 1)->runLength = lastChunkSize;

    /* chunkNumber = heapSize */
    merger->heapSize = 0;
    merger->heapCapacity = chunkNumber;
    merger->inputFP = inputFP;
    merger->outputFP = outputFP;
    merger->outputBuffer = outputBuffer;
    merger->currentPositionInOutputBuffer = 0;
    merger->outputBufferCapacity = elementsPerBuffer;
    merger->inputBuffers = inputBuffers;
    
    
    return 0;
}


int mergeRuns(MergeManager *merger) {
    /* 1. init inputBuffers */
    if (initInputBuffers(merger)!=0) {
        fprintf(stderr, "Error in inputBuffer init!\n");
        exit(0);
    }

    /* 2. init heap */
    if (initHeap(merger)!=0) {
        fprintf(stderr, "Error in heap init!\n");
        exit(0);
    }

    while (merger->heapSize > 0) { /* heap is not empty */
        HeapRecord *smallest;
        Record *next;

        int runID, uid1, uid2;

        if (getTopHeapElement(merger, smallest)!=0) {
            fprintf(stderr, "Failed to get the smallest element in the heap!\n");
            exit(0);
        }

        /* add record to outputBuffer */
        runID = smallest->run_id;
        uid1 = smallest->uid1;
        uid2 = smallest->uid2;
        int currentPosition = merger->currentPositionInOutputBuffer++;
        Record *outputBuffer = merger->outputBuffer;
        (outputBuffer+currentPosition)->uid1 = uid1;
        (outputBuffer+currentPosition)->uid2 = uid2;

        /* get the next Record from inputBuffer */
        result = getNextRecord(merger, runID, next);
        
        /* if next element exists */
        if (next != NULL) {
            /* insert into heap */
            if (insertIntoHeap(merger, smallest->run_id, next)!=0) {
                return 1;
            }
        }
    }
}




