#include "disk_ram.h"

FILE *fp_read;


int write_lines(char *input_file) {
    FILE *fp_write;

    char *file_path = "test_dataset/g_plusAnonymized.csv";

    /* initialize read buffer */
    unsigned long total_lines = get_file_lines(file_path);
    Record *read_buffer = (Record *)malloc(sizeof(Record)*total_lines);

    /* initialize write buffer */
    Record *write_buffer = read_buffer;

    if (!(fp_write = fopen(input_file, "wb"))) {
        printf("Couldn't open file \"%s\" for writing! \n", input_file);
        exit(0);
    }

    read_init("test_dataset/g_plusAnonymized.csv");
    read_entire_file(read_buffer);
    
    unsigned long write_buffer_pos = 0;

    while (write_buffer_pos <= total_lines) {
        /* point the write buffer to next position */
        write_buffer = (Record *)(read_buffer + write_buffer_pos);
        /* flush write buffer */
        fwrite(write_buffer, sizeof(Record), 1, fp_write);
        /* update write buffer position */
        write_buffer_pos += 1;
    }

    fclose(fp_read);
    fclose(fp_write);

    free(read_buffer);

    return 1;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("write_lines <input filename>\n");
        return (-1);
    }
    char *file_name = argv[1];
    if (write_lines(file_name) != 1) {
        printf("write file failed!\n");
        return (-1);
    }
    return 1;
}

