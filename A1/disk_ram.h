#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct record {
    int uid1;
    int uid2;
} Record;

unsigned int get_lines(FILE *file);
Record* store_record();
