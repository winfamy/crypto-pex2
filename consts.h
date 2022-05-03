#pragma once
#include <stdio.h>
#include <stdlib.h>

void ERR(const char * s) {
    fprintf(stderr,"%s\n",s);
    exit(EXIT_FAILURE);
}

const int KB = 1024;
const int MB = 1024 * 1024;
const int MAX_THREADS_PER_BLOCK = 1024;
const int MAX_BLOCKS_PER_GRID = 65535;
