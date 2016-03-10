#include <stdio.h>
#include <stdlib.h>
#include "Merge.h"

//sample multiway merge template
int mergeRuns (MergeManager *merger) { //merger should be already set up at this point
	int  result;
	//1. go in the loop through all input files and fill-in initial buffers
	if (initInputBuffers(merger)!=0)
		return 1;
	
	//2. Initialize heap with 1 element from each buffer
	if (initHeap(merger)!=0)
		return 1;	
	
	while (merger->heapSize > 0) { //heap is not empty	
		HeapRecord smallest;
		Record next;
		
		int runID;
		
		if (getTopHeapElement (merger, &smallest)!=0)
			return 1;	
		runID = smallest.run_id;
		
		merger->outputBuffer[merger->currentPositionInOutputBuffer++]=smallest;		

		result = getNextRecord (merger, runID, &next);

		if(next != NULL) {//next element exists		
			if(insertIntoHeap (merger, smallest.runID, &next)!=0)
				return 1;
		}
		if(result==1) //error
			return 1;
		
		if(merger->currentPositionInOutputBuffer == merger-> outputBufferCapacity ) { //staying on the last slot of the output buffer - next will cause overflow
			if(flushOutputBuffer(merger)!=0)
				return 1;			
			merger->currentPositionInOutputBuffer=0;
		}	
	}
	//flush what remains in output buffer
	if(merger->currentPositionInOutputBuffer >0) { //there are elements in the output buffer	
		if(flushOutputBuffer(merger)!=0)
			return 1;
	}

	return 0;	
}

