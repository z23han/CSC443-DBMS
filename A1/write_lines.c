#include "disk_ram.h"

FILE *fp_read;


int write_lines(char *input_file) {
    FILE *fp_write;
    int tokens_found;

    Record *buffer = (Record *)malloc(sizeof(Record));

    if (!(fp_write = fopen(input_file, "wb"))) {
        printf("Couldn't open file \"%s\" for writing! \n", input_file);
        exit(0);
    }

    read_init("test_dataset/g_plusAnonymized.csv");
    
    while (1) {
        tokens_found = read_file(1, buffer);
        if (tokens_found == 0) {
            break;
        }
        fwrite(buffer, sizeof(Record), 1, fp_write);
        fflush(fp_write);
    }

    fclose(fp_read);
    fclose(fp_write);

    free(buffer);

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

