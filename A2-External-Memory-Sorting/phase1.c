/* 
 *  partition input into K chunks, and sort by uid2, and write to disk
 *  each chunk fit into memory, while each chunk is dissected into several blocks
 * */

#include "Merge.h"


void sort_chunks(char *input_file, char *output_file, int total_mem, int block_size, int num_runs) {
    FILE *fp_read, *fp_write;
    
    if (!(fp_read = fopen(input_file, "rb"))) {
        fprintf(stderr, "Couldn't read file \"%s\"\n", input_file);
        exit(0);
    }

    if (!(fp_write = fopen(output_file, "wb"))) {
        fprintf(stderr, "Couldn't write file \"%s\"\n", output_file);
        exit(0);
    }
    
    /* get the size of the file */
    fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET);

    /* make sure the chunk size is smaller than memory size */
    int one_chunk = file_size / num_runs;
    /* adjust the one chunk size to fit integer number of Record */
    int remainder = one_chunk % sizeof(Record);
    int chunk_size;
    if (remainder == 0) {
        chunk_size = one_chunk;
        }
    } else {
        chunk_size = one_chunk - remainder + sizeof(Record);
    }
    if (chunk_size > total_mem) {
        fprintf(stderr, "chunk size bigger than memory size!\n");
        exit(0);
    }
    /* allocate a buffer holding each chunk */
    char *buffer = (char *)malloc(chunk_size);

    /* create a new sorting manager */
    SortingManager *sm = (SortingManager *)malloc(sizeof(SortingManager));
    sm->partitionBuffer = (Record *)buffer;
    sm->inputFile = fp_read;
    sm->totalRecords =  
    
}



