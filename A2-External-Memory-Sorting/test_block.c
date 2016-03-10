/*
 *  read input file into the buffer and sort records by uid2
 * */

#include "Merge.h"


/* compare a and b records */
int compare(const void *a, const void *b) {
    int a_uid2 = ((const Record*)a)->uid2;
    int b_uid2 = ((const Record*)b)->uid2;
    return (a_uid2 - b_uid2);
}


void read_buffer(int buffer_size, char *input_file) {
    FILE *fp_read;

    if (!(fp_read = fopen(input_file, "rb"))) {
        fprintf(stderr, "Couldn't read file \"%s\"\n", input_file);
        exit(0);
    }

    /* malloc buffer and read into the buffer */
    char *buffer = (char *)malloc(buffer_size);
    int result = fread(buffer, 1, buffer_size, fp_read);

    /* get the total record number */
    int total_records = buffer_size / sizeof(Record);
    qsort(buffer, total_records, sizeof(Record), compare);

    /* print to the stdout */
    int i, uid1, uid2;
    for (i = 0; i < total_records; i++) {
        uid1 = ((Record *)(buffer + sizeof(Record)*i))->uid1;
        uid2 = ((Record *)(buffer + sizeof(Record)*i))->uid2;
        fprintf(stdout, "uid1: %d, uid2: %d\n", uid1, uid2);
    }

    free(buffer);
    
}


void main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "test_block <block_size>\n");
        exit(0);
    }
    int block_size = atoi(argv[1]);
    char *input_file = "edges.dat";
    read_buffer(block_size, input_file);
}


