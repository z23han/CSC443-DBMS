/* 
 *  partition input into K chunks, and sort by uid2, and write to disk
 *  each chunk fit into memory, while each chunk is dissected into several blocks
 * */

#include "Merge.h"

/* 
 *  block_size is 8K (8192)
 * */
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
    /* adjust the one chunk size to fit the block size */
    int remainder = one_chunk % block_size;
    int chunk_size;
    if (remainder == 0) {
        chunk_size = one_chunk;
    } else {
        chunk_size = one_chunk - remainder + block_size;
    }
    if (chunk_size > total_mem) {
        fprintf(stderr, "chunk size bigger than memory size!\n");
        exit(0);
    }
    /* allocate a buffer holding each chunk */
    char *buffer = (char *)calloc(chunk_size, 1);

    /* create a new sorting manager */
    SortingManager *sm = (SortingManager *)calloc(sizeof(SortingManager), 1);
    sm->partitionBuffer = (Record *)buffer;
    sm->inputFile = fp_read;
    sm->outputFile = fp_write;
    sm->totalRecords = chunk_size;
    sm->totalPartitions = num_runs;
    
    int res = makeRun(sm);
    if (res != 0) {
        fprintf(stderr, "Failed to sort the input file \"%s\"\n", input_file);
        exit(0);
    }

    close(fp_read);
    close(fp_write);

    free(buffer);
}

/* compare a and b records */
int compare(const void *a, const void *b) {
    int a_uid2 = ((const Record*)a)->uid2;
    int b_uid2 = ((const Record*)b)->uid2;
    return (a_uid2 - b_uid2);
}

/* 
 *  sort and write to the disk
 * */
int makeRun(SortingManager *manager) {
    /* obtain the members from the manager */
    FILE *fp_read = manager->inputFile;
    FILE *fp_write = manager->outputFile;
    Record *buffer = manager->partitionBuffer;
    int chunk_size = manager->totalRecords;
    int partition_num = manager->totalPartitions;

    int records_per_chunk = chunk_size / sizeof(Record);
    /* sort each partition */
    int i, result, j;
    for (i = 0; i < partition_num; i++) {
        /* load into buffer a chunk of data */
        result = fread(buffer, sizeof(Record), records_per_chunk, fp_read);
        /*
        if (result != records_per_chunk) {
            if (i != partition_num-1) {
                fprintf(stderr, "read file error! Not equal to chunk size\n");
                exit(0);
            }
        }
        */
        fwrite(buffer, sizeof(Record), result, fp_write);
        fflush(fp_write);
        /* break if we reach the end of the file */
        if (result != records_per_chunk) {
            break;
        }
    }
    return 0;
}


void main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "phase1 <input file> <output file> <total memory> <block size> <number of runs>\n");
        exit(0);
    }
    char *inputFile = argv[1];
    char *outputFile = argv[2];
    int total_memory = atoi(argv[3]);
    int block_size = atoi(argv[4]);
    int num_runs = atoi(argv[5]);

    sort_chunks(inputFile, outputFile, total_memory, block_size, num_runs);

}

