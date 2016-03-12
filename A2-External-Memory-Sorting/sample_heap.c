int getTopHeapElement (MergeManager *merger, HeapRecord *result) {
	Record item;
	int child, parent;

	if(merger->heapSize == 0) {
		printf( "UNEXPECTED ERROR: popping top element from an empty heap\n");
		return 1;
	}

	*result=merger->heap[0];  //to be returned

	//now we need to reorganize heap - keep the smallest on top
	item = merger->heap [-- merger->heapSize]; // to be reinserted 

	parent =0;

	while ((child = (2 * parent) + 1) < merger->currentHeapSize) {
		// if there are two children, compare them 
		if (child + 1 < merger->heapSize && (compare((void *)&(merger->heap[child]),(void *)&(merger->heap[child + 1]))>0)) {
			++child;
		}
		// compare item with the larger 
		if (compare((void *)&item, (void *)&(merger->heap[child]))>0) {
			merger->heap[parent] = merger->heap[child];
			parent = child;
		} else {
			break;
		}
	}
	merger->heap[parent] = item;
	
	return 0;
}

int insertIntoHeap (MergeManager *merger, int run_id, Record *record) {
	HeapRecord hrecord;
	hrecord.uid1 = record ->uid1;
	hrecord.uid2 = record ->uid2;
	hrecord.run_id = run_id;
	
	int child, parent;
	if (merger->heapSize == merger->heapCapacity)	{
		printf( "Unexpected ERROR: heap is full\n");
		return 1;
	}	
  	
	child = merger->heapSize++; /* the next available slot in the heap */
	
	while (child > 0)	{
		parent = (child - 1) / 2;
		if (compare((void *)&(merger->heap[parent]),(void *)hrecord)>0) {
			merger->heap[child] = merger->heap[parent];
			child = parent;
		} else 	{
			break;
		}
	}
	merger->heap[child]= *hrecord;	
	return 0;
}
