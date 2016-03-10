#include "disk_ram.h"
#include <sys/timeb.h>


/* 
 *  Get the max and average of the user
 * */
void get_max_ave(Record *buffer, unsigned long num_of_records, FILE *fp_write, User *old_user) {
    int i;
    Record *new_record;
    int new_uid, new_uid2;
    int ave;
    for (i = 0; i < num_of_records; i++) {
        Record *new_record = (Record *)(buffer + i);
        new_uid = new_record->uid1;
        new_uid2 = new_record->uid2;
        /* if new_uid==old_uid, check and update user information */
        if (new_uid == old_user->uid) {
            /* check max value */
            if (new_uid2 > old_user->max) {
                old_user->max = new_uid2;
            }
            /* update num and sum */
            old_user->sum += new_uid2;
            old_user->num += 1;
        }/* else write to the disk and create a new user */ else {
            if (old_user->uid != 0) {
                ave = old_user->sum / old_user->num;
                fprintf(fp_write, "uid: %ld  max: %ld  ave: %ld\n", old_user->uid, old_user->max, ave);
            }
            old_user->uid = new_uid;
            old_user->max = new_uid2;
            old_user->sum = new_uid2;
            old_user->num = 1;
        }
    } 
}


int seq_access(unsigned long block_size, char* file_name) {
    unsigned long records_per_block = block_size/sizeof(Record);
    int time_spent_ms;
    struct timeb t_begin, t_end;

    FILE *fp_read, *fp_write;

    Record *buffer = (Record *)calloc(records_per_block, sizeof(Record));

    if (!(fp_read = fopen(file_name, "rb"))) {
        printf("Couldn't open the file \"%s\" for reading!\n", file_name);
        exit(0);
    }

    char *write_file = "max_ave_disk.cvs";
    if (!(fp_write = fopen(write_file, "wb"))) {
        printf("Couldn't open the file \"%s\" for writing!\n", write_file);
        exit(0);
    }

    /* initialize user struct */
    User* record_user = (User *)malloc(sizeof(User));
    record_user->uid = 0;
    record_user->max = 0;
    record_user->sum = 0;
    record_user->num = 0;
    unsigned long uid;

    unsigned long count = 0;
    /* start timing */
    ftime(&t_begin);
    while (1) {
        unsigned long result = fread(buffer, sizeof(Record), records_per_block, fp_read);
        /* get_max_ave function write the record information to the disk for 1 block */
        get_max_ave(buffer, result, fp_write, record_user);
        if (result != records_per_block) {
            break;
        }
        count += 1;
    }
    ftime(&t_end);

    time_spent_ms = (int)(1000*(t_end.time-t_begin.time) + (t_end.millitm-t_begin.millitm));
    printf("count: %ld\n", count);
    printf("Data rate: %.3f MBPS\n", ((count*sizeof(Record))/(float)time_spent_ms*1000)/MB);

    fclose(fp_read);

    free(record_user);
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


