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
        
        /* else load new data into buffer from inputFP */
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
        inputBuffer->currentPositionInFile += loadSize;
        inputBuffer->currentBufferPosition = 0;
        inputBuffer->totalElements += loadSize;
        inputBuffer->buffer = (Record *)buffer;
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
        if (pushIntoHeap(heap, newRecord, run_id)!=0) {
            fprintf(stderr, "Failed to push into heap!\n");
            exit(0);
        }
        /* update merger information */
        merger->heapSize++;
        
    } 
    /* heap size should be smaller than capacity */
    else  {
        fprintf(stderr, "heap size larger than heap capacity! Error!\n");
        exit(0);
    }

    return 0;
}


/* push element into the heap */
int pushIntoHeap(HeapRecord *heap, Record *oneRecord, int runID) {

    int uid2 = oneRecord->uid2;
    HeapRecord *first = heap;

    if (first == NULL) {
        first->uid1 = oneRecord->uid1;
        first->uid2 = oneRecord->uid2;
        first->run_id = runID;
    }

    while (first != NULL) {
        if (first->uid2 > uid2) {
            if (addHeapRecordToTop(first, oneRecord, runID)!=0) {
                fprintf(stderr, "Failed to insert into heap!\n");
                exit(0);
            }
            return 0;
        }
        first++;
    }
    /* add to the end */
    first->uid1 = oneRecord->uid1;
    first->uid2 = oneRecord->uid2;
    first->run_id = runID;

    return 0;
}


/* add the element from record to the top of the heap */
int addHeapRecordToTop(HeapRecord *heap, Record *oneRecord, int runID) {

    int uid1_next, uid2_next, uid1_now, uid2_now, run_id_next, run_id_now;
    uid1_next = heap->uid1;
    uid2_next = heap->uid2;
    run_id_next = heap->run_id;
    uid1_now = oneRecord->uid1;
    uid2_now = oneRecord->uid2;
    run_id_now = runID;
    heap->uid1 = uid1_now;
    heap->uid2 = uid2_now;
    heap->run_id = run_id_now;
    
    heap = heap + 1;
    /* loop the rest of the heap and update uid */
    do {
        /* store uid1 and uid2 to a place */
        uid1_now = uid1_next;
        uid2_now = uid2_next;
        run_id_now = run_id_next;
        uid1_next = heap->uid1;
        uid2_next = heap->uid2;
        run_id_next = heap->run_id;
        heap->uid1 = uid1_now;
        heap->uid2 = uid2_now;
        heap->run_id = run_id_now;
        
        heap ++;
    } while(heap != NULL);

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



