#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHARS_PER_LINE 256
#define KB 1024
#define MB 1048576


typedef struct record {
    unsigned long uid1;
    unsigned long uid2;
} Record;


typedef struct user {
    unsigned long uid;
    unsigned long max;
    unsigned long sum;
    unsigned long num;
} User;


extern FILE *fp_read;

void read_init(char *input_file);
unsigned long get_file_lines(char *input_file);
int read_file_block(unsigned long records_per_block, Record *buffer);
void read_entire_file(Record *buffer);
int write_file(char *input_file, unsigned long size);
int write_lines(char *input_file);
int seq_access(unsigned long block_size, char* file_name);
int seq_ram(char* file_name);
void get_max_ave(Record *buffer, unsigned long num_of_records, FILE *fp_write, User *old_user);
void get_max_ave2(Record *buffer, unsigned long file_lines, FILE *fp_write);


