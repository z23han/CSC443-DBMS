#include "disk_ram.h"
#include <sys/timeb.h>


void get_max_ave2(Record* buffer, unsigned long file_lines, FILE *fp_write) {
    unsigned long buffer_ptr_pos = 0;
    User* old_user = (User *)malloc(sizeof(User));
    old_user->uid = 0;
    old_user->max = 0;
    old_user->sum = 0;
    old_user->num = 0;
    unsigned long new_uid, new_uid2, average;
    unsigned long index = 0;
    unsigned long buffer_used = 0;
    while (buffer_used < file_lines) {
        Record *new_record = (Record *)(buffer + index);
        new_uid = new_record->uid1;
        new_uid2 = new_record->uid2;
        /* if uid equals, updating information */
        if (new_uid == old_user->uid) {
            /* check the maximum */
            if (new_uid2 > old_user->max) {
                old_user->max = new_uid2;
            }
            old_user->sum += new_uid2;
            old_user->num += 1;
        } else /* write the the disk and reinitialize user */ {
            if (old_user->uid != 0) {
                average = old_user->sum / old_user->num;
                fprintf(fp_write, "uid: %ld  max: %ld  ave: %ld\n", old_user->uid, old_user->max, average);
            }
            old_user->uid = new_uid;
            old_user->max = new_uid2;
            old_user->sum = new_uid2;
            old_user->num = 1;
        }
        buffer_used += sizeof(Record);
        index ++;
    }
    free(old_user);
}


int seq_ram(char *file_name) {
    int time_spent_ms;
    struct timeb t_begin, t_end;
    char *buffer;
    unsigned long fileLen;

    FILE *fp_read, *fp_write;

    /* file pointer for reading file */
    if (!(fp_read = fopen(file_name, "rb"))) {
        printf("Read file \"%s\" error!\n", file_name);
        exit(0);
    }

    /* file pointer for writing to the disk */
    char *write_file = "max_ave_ram.cvs";
    if (!(fp_write = fopen(write_file, "wb"))) {
        fprintf(stderr, "Write file \"%s\" failed!\n", write_file);
        exit(0);
    }

    /* Get file length */
    fseek(fp_read, 0, SEEK_END);
    fileLen = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET);

    buffer = (char *)malloc(fileLen + 1);
    if (!buffer) {
        fprintf(stderr, "Memory error!\n");
        fclose(fp_read);
        exit(0);
    }


    /* measure time for reading and getting max&ave */
    ftime(&t_begin);
    int result = fread(buffer, 1, fileLen, fp_read);
    if (result != fileLen) {
        fprintf(stderr, "Error reading binary file into ram!\n");
        exit(0);
    }
    get_max_ave2((Record *)buffer, fileLen, fp_write);
    ftime(&t_end);

    time_spent_ms = (int)(1000*(t_end.time-t_begin.time) + (t_end.millitm-t_begin.millitm));
    printf("Data rate: %.3f MBPS\n", ((fileLen*sizeof(Record))/(float)time_spent_ms*1000)/MB);
    fclose(fp_read);

    free(buffer);
    return 1;
}

int main(int argc, char* argv[]) {
    char *file_name = "records.dat";
    if (seq_ram(file_name)!= 1) {
        printf("read file failed!\n");
        exit(0);
    }
    return 1;
}
