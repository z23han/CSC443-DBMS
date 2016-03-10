#include <stdlib.h>
#include "Merge.h"


/* compare 2 records a and b, returning an integer indicating order */
int compare(const void *a, const void *b) {
    int a_f = ((const Record*)a)->uid2;
    int b_f = ((const Record*)b)->uid2;
    return (a_f - b_f);
}


/* 
 * 1 - array to sort
 * 2 - size of an array
 * 3 - size of each array element
 * 4 - function to compare 2 elements of the array
 * qsort(buffer, total_records, sizeof(Record), compare); 
 *
 * */


