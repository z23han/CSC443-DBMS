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


extern Record* user_record;

int get_lines();
int read_file();


