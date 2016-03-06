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


extern FILE *fp_read;

void read_init(char *input_file);
int read_file(int records_per_block, Record *buffer);
int write_file(char *input_file, int size);
int write_lines(char *input_file);

