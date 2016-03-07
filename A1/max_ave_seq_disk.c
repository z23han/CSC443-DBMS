#include "disk_ram.h"
#include <sys/timeb.h>

int seq_access(int block_size, char* file_name) {
    int records_per_block = block_size/sizeof(Record);
    int time_spent_ms;
    struct timeb t_begin, t_end;

    FILE *fp_read;

    Record *buffer = (Record *)calloc(records_per_block, sizeof(Record));

    if (!(fp_read = fopen(file_name, "rb"))) {
        printf("Couldn't open the file \"%s\" for reading!\n", file_name);
        exit(0);
    }

    int count = 0;
    /* start timing */
    ftime(&t_begin);
    while (1) {
        int result = fread(buffer, sizeof(Record), records_per_block, fp_read);
        if (result != records_per_block) {
            break;
        }
        count += 1;
    }
    ftime(&t_end);

    time_spent_ms = (int)(1000*(t_end.time-t_begin.time) + (t_end.millitm-t_begin.millitm));
    printf("count: %d\n", count);
    printf("Data rate: %.3f MBPS\n", ((count*sizeof(Record))/(float)time_spent_ms*1000)/MB);

    fclose(fp_read);

    free(buffer);

    return 1;
}


int main(int argc, char *argv[]) {
    char *file_name = "records.dat";
    if (seq_access(512, file_name) != 1) {
        printf("read file failed!\n");
        exit(0);
    }
    return 1;
}


