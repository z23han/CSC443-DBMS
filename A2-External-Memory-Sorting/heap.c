/* 
 *  heap operations
 * */

#include "Merge.h"


/* inserts into heap one element from each buffer - to keep the smallest on top */
int initHeap(MergeManager *merger) {

    InputBuffer *inputBuffers = merger->inputBuffers;   /* inputBuffer array pointer */
    int heapCapacity = merger->heapCapacity;    /* max number of elements heap can hold */
    int heapSize = merger->heapSize;            /* current number of elements in the heap */
    if (heapSize != 0) {
        fprintf(stderr, "heap not empty! Error!\n");
        exit(0);
    }
    HeapRecord *heap = merger->heap;
    /* loop through each inputBuffer and insert one element into heap */
    int i, uid1, uid2;
    InputBuffer *currentBuffer;
    Record *currentRecord;
    for (i = 0; i < heapCapacity; i++) {
        
        currentBuffer = (InputBuffer*)(inputBuffers + i);
        currentRecord = currentBuffer->buffer;

        uid1 = currentRecord->uid1;
        uid2 = currentRecord->uid2;
        /* updating heap */
        (heap + i)->uid1 = uid1;
        (heap + i)->uid2 = uid2;
        (heap + i)->run_id = i;
        
        /* updating buffer variables */
        currentBuffer->currentPositionInFile += 1;
        currentBuffer->currentBufferPosition += 1;
        
    }
    merger->heapSize = heapCapacity;

    return 0;
}


/* reads the next element from an input buffer 
 *  return 0 meaning successful, return 1 meaning chunk data exhausted
 * */
int getNextRecord(MergeManager *merger, int run_id, Record *result) {

    /* get Record */
    InputBuffer *inputBuffer = merger->inputBuffers + run_id;
    Record *buffer = inputBuffer->buffer;
    result = buffer + currentBufferPosition;
    
    /* update inputBuffer */
    buffer->currentBufferPosition++;

    /* if the buffer is empty, load data into buffer */
    if (buffer->currentBufferPosition == buffer->capacity) {

        /* if we exhaust the chunk */
        if (inputBuffer->totalElements == inputBuffer->runLength) {
            inputBuffer->done = 1;
            return 1;
        }
        
        /* to be continued ....... */
        FILE *inputFP = merger->inputFP;
        int filePosition = inputBuffer->currentPositionInFile;
        fseek(inputFP, filePosition, SEEK_SET);
        size_t loadSize = fread(buffer, inputBuffer->capacity, sizeof(Record), inputFP);
        if (loadSize){
            fprintf(stderr, "Unable to load data to input buffer!\n");
            exit(0);
        }
        fseek(inputFP, 0, SEEK_SET);
        /* update inputBuffer */
    }

    return 0;
}



/* insert next element from run_id into heap */
int insertIntoHeap(MergeManager *merger, int run_id, Record *newRecord) {
    HeapRecord *heap = merger->heap;
    int heapSize = merger->heapSize;
    int heapCapacity = merger->heapCapacity;
    /* if heap size is smaller than heap capacity, just insert */
    if (heapSize < heapCapacity) {
        
    } 
    /* else if heap size equals to heap capacity, insert and remove */
    else if (heapSize == heapCapacity) {
        
    }
    else {
        fprintf(stderr, "heap size larger than heap capacity! Error!\n");
        exit(0);
    }

    return 0;
}


/* removes the smallest element from the heap, and restores heap order */
int getTopHeapElement(MergeManager *merger, HeapRecord *result) {
    
    Record *item;

    if(merger->heapSize == 0) {
        fprintf(stderr, "Pop element from an empty heap!\n");
        exit(0);
    }

    HeapRecord *first = merger->heap;
    HeapRecord *second = merger->heap + 1;
   
    result = first;
   
    /* update merger */
    merger->heap = second;
    merger->heapSize--;
   
    return 0;
}



