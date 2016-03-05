#include "disk_ram.h"

/* return the number of lines */
int get_lines() {
    int line_number = 0;
    FILE *fp_read;
    char current_line[MAX_CHARS_PER_LINE];

    char *file_name = "/test_dataset/g_plusAnonymized.csv";
    /* open file for reading */
    if (!(fp_read=fopen(file_name, "r"))) {
        printf("Couldn't open file \"%s\" for reading \n", file_name);
        return (-1);
    }
    
    /* record the number of lines */
    while (fgets(current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL) {
        line_number += 1;
    }

    fclose(fp_read);

    return line_number;
}


/* return -1 if there is an error */
int read_file() {
    char current_line[MAX_CHARS_PER_LINE];
    FILE *fp_read;
    char *tokens;
    int uid_mark=1, count=0;

    char *file_name = "/test_dataset/g_plusAnonymized.csv";

    /* open text file for reading */
    if (!(fp_read=fopen(file_name, "r"))) {
        printf("Couldn't open file \"%s\" for reading \n", file_name);
        return (-1);
    }

    /* initialize user record to store all the lines */
    Record *user_record = (Record *)malloc(sizeof(Record) * get_lines());

    /* reading lines */
    while (fgets(current_line, MAX_CHARS_PER_LINE, fp_read)!= NULL) {
        tokens = strtok(current_line, ",");
        while (tokens) {
            if (uid_mark==1) {
                (user_record+count)->uid1 = atoi(tokens);
            } else {
                (user_record+count)->uid2 = atoi(tokens);
            }
            uid_mark += 1;
            tokens = strtok(NULL, " ");
        }
        uid_mark = 1;
        count += 1;
    }

    fclose(fp_read);
    
    return (1);
}

void main() {
    read_file();
}
