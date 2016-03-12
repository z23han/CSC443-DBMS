#include "Merge.h"


/* init input buffers by filling with elements */
int initInputBuffers(MergeManager *merger) {
    InputBuffer *inputBuffers = merger->inputBuffers;
    FILE *inputFP = merger->inputFP;
    int heapCapacity = merger->heapCapacity;
    int outputBufferCapacity = merger->outputBufferCapacity;

    /* fill every buffer */
    int i;
    InputBuffer *currentBuffer;
    /* get the previous chunk size for next round file position allocation */
    int filePosition = inputBuffers->runLength;
    
    for (i = 0; i < heapCapacity; i++) {
        currentBuffer = (InputBuffer*)(inputBuffers + i);
        currentBuffer->capacity = outputBufferCapacity;
        currentBuffer->currentPositionInFile = filePosition;
        currentBuffer->currentBufferPosition = 0;
        currentBuffer->totalElements = outputBufferCapacity;
        currentBuffer->done = 0;
        currentBuffer->buffer = (Record *)calloc(outputBufferCapacity, sizeof(Record));
        /* search for the file position and read the file */
        fseek(inputFP, filePosition, SEEK_SET);
        fread(currentBuffer->buffer, currentBuffer->capacity, sizeof(Record), inputFP);
        fseek(inputFP, 0, SEEK_SET);
        /* updating the chunk size */
        filePosition += currentBuffer->runLength;
    }

    return 0;
}


