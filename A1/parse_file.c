#include "disk_ram.h"


Record *user_record;


void main() {
    user_record = store_record();
}


unsigned int get_lines(FILE *file) {
    char line[256];
    unsigned int count = 0;
    while (fgets(line, sizeof(line), file)) {
        count += 1;
    }
    return count;
}

Record* store_record() {
    FILE *fp;
    char line[256];
    int uid_mark=1, count=0;
    
    /* file open */
    fp = fopen("/test_dataset/g_plusAnonymized.csv", "r");
    unsigned int num_of_lines = get_lines(fp);
    /* re-open the file */
    fp = fopen("/test_dataset/g_plusAnonymized.csv", "r");
    
    /* allocate pointer with num_of_lines length */
    user_record = (Record *)malloc(sizeof(Record)*num_of_lines);

    char *tokens;

    while (fgets(line, sizeof(line), fp)) {
        tokens = strtok(line, ",");
        while (tokens) {
            if (uid_mark == 1) {
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


    fclose(fp);

    return user_record;
}

