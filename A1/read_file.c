#include "disk_ram.h"

FILE *fp_read;

/* initialize fp_read file */
void read_init(char* input_file) {
    if (!(fp_read=fopen(input_file, "r"))) {
        printf("Couldn't open file \"%s\" for reading! \n", input_file);
        exit(0);
    }
}


int get_file_lines(char *input_file) {
    FILE *fp;
    int lines = 0;
    char current_line[MAX_CHARS_PER_LINE];

    if (!(fp = fopen(input_file, "r"))) {
        printf("Couldn't open file!\n");
        exit(0);
    }

    while (fgets(current_line, MAX_CHARS_PER_LINE, fp)!= NULL) {
        lines += 1;
    }

    return lines;
}

/* 
 *  read the entire file
 * */
void read_entire_file(Record *buffer) {
    char current_line[MAX_CHARS_PER_LINE];
    char *tokens;
    int uid_mark = 1, count = 0;

    /* read lines */
    while (fgets(current_line, MAX_CHARS_PER_LINE, fp_read)!= NULL) {
        tokens = strtok(current_line, ",");
        while (tokens) {
            if (uid_mark == 1) {
                (buffer+count)->uid1 = atoi(tokens);
            } else {
                (buffer+count)->uid2 = atoi(tokens);
            }
            uid_mark += 1;
            tokens = strtok(NULL, " ");
        }
        uid_mark = 1;
        count += 1;
    }
}


/* read a block of records from the file and store into the buffer */
int read_file_block(int records_per_block, Record *buffer) {
    char current_line[MAX_CHARS_PER_LINE];
    char *tokens;
    int uid_mark=1, count=0;

    int tokens_found = 0;

    /* reading lines */
    while (fgets(current_line, MAX_CHARS_PER_LINE, fp_read)!= NULL) {
        if (tokens_found == 0) {
            tokens_found = 1;
        }
        tokens = strtok(current_line, ",");
        while (tokens) {
            if (uid_mark==1) {
                (buffer+count)->uid1 = atoi(tokens);
            } else {
                (buffer+count)->uid2 = atoi(tokens);
            }
            uid_mark += 1;
            tokens = strtok(NULL, " ");
        }
        uid_mark = 1;
        count += 1;
        if (count == records_per_block) {
            break;
        }
    }

    return tokens_found;

}




