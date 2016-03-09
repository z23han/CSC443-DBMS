#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHARS_PER_LINE 256
#define KB 1024
#define MB 1048576


typedef struct record {
    int uid1;
    int uid2;
} Record;


typedef struct user {
    int uid;
    int max;
    int sum;
    int num;
} User;


extern FILE *fp_read;

void read_init(char *input_file);
int get_file_lines(char *input_file);
int read_file_block(int records_per_block, Record *buffer);
void read_entire_file(Record *buffer);
int write_file(char *input_file, int size);
int write_lines(char *input_file);
int seq_access(int block_size, char* file_name);
int seq_ram(char* file_name);
void get_max_ave(Record *buffer, int num_of_records, FILE *fp_write, User *old_user);


