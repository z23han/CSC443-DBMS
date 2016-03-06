#include "disk_ram.h"

FILE *fp_read;


/* return -1 if failed to write to the buffer */
int write_file(char* input_file, int block_size) {
    /*int sizes[8] = {512, KB, 4*KB, 16*KB, 21*KB, 1*MB, 2*MB, 4*MB};*/
    int records_per_block;
    FILE *fp_write;
    int tokens_found;
    char *file_name;
    Record *buffer;

    /* initialize the block */
    records_per_block = block_size/sizeof(Record);
    /* allocate buffer for 1 block */
    buffer = (Record *)calloc(records_per_block, sizeof(Record));

    if (!(fp_write = fopen(input_file, "wb"))) {
        printf("Couldn't open file \"%s\" for writing \n", input_file);
        exit(0);
    }

    read_init("test_dataset/g_plusAnonymized.csv");
    int count = 0;

    while (1) {
        tokens_found = read_file(records_per_block, buffer);
        /*printf("tokens_found: %d\n", tokens_found);*/
        if (tokens_found == 0) {
            break;
        }
        count += 1;
        /* flush buffer when full */
        fwrite(buffer, sizeof(Record), records_per_block, fp_write);
        /* force data to disk */
        fflush(fp_write);
    }
    printf("count: %d\n", count);

    fclose(fp_read);
    fclose(fp_write);

    free(buffer);

    return 1;

}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("write_blocks <input filename> <block size>\n");
        return (-1);
    }

    char* file_name = argv[1];
    int block_size = atoi(argv[2]);
    if (write_file(file_name, block_size)!=1) {
        printf("write file failed!\n");
        return (-1);
    }
    return 1;
}


