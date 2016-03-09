#include "disk_ram.h"

FILE *fp_read;


/* return -1 if failed to write to the buffer */
int write_file(char* input_file, int block_size) {
    /*int sizes[8] = {512, KB, 4*KB, 16*KB, 21*KB, 1*MB, 2*MB, 4*MB};*/
    int records_per_block;
    FILE *fp_write;
    Record *read_buffer, *write_buffer;

    char *file_path = "test_dataset/g_plusAnonymized.csv";

    /* initialize read buffer */
    int total_lines = get_file_lines(file_path);
    read_buffer = (Record *)malloc(sizeof(Record)*total_lines);

    /* initialize write buffer */
    records_per_block = block_size/sizeof(Record);
    write_buffer = read_buffer;

    /* open file for writing */
    if (!(fp_write = fopen(input_file, "wb"))) {
        printf("Couldn't open file \"%s\" for writing \n", input_file);
        exit(0);
    }

    /* read the file */
    read_init(file_path);
    read_entire_file(read_buffer);

    int write_buffer_pos = 0;

    while (write_buffer_pos <= total_lines) {
        /* point the write buffer to the correct position */
        write_buffer = (Record *)(read_buffer + write_buffer_pos);
        /* flush write buffer */
        fwrite(write_buffer, sizeof(Record), records_per_block, fp_write);
        /* force write buffer to disk */
        fflush(fp_write);
        /* update write buffer position */
        write_buffer_pos += records_per_block;
    }

    fclose(fp_read);
    fclose(fp_write);

    free(read_buffer);

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


