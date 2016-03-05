#include "disk_ram.h"

/* return -1 if failed to write to the buffer */
int write_file(char* input_file, int size) {
    /*int sizes[8] = {512, KB, 4*KB, 16*KB, 21*KB, 1*MB, 2*MB, 4*MB};*/
    int block_size = size;
    int records_per_block = block_size/sizeof(Record);
    FILE *fp_write;

    /* allocate buffer for 1 block */
    Record *buffer = (Record *)calloc(records_per_block, sizeof(Record));

    file_name = input_file;

    if (!(fp_write = fopen(file_name, "wb"))) {
        printf("Couldn't open file \"%s\" for writing \n", file_name);
        return (-1);
    }

    /* flush buffer when full */
    fwrite(buffer, sizeof(Record), total_records, fp_write);

    /* force data to disk */
    fflush(fp_write);

    fclose(fp_write);

    free(buffer);

}

