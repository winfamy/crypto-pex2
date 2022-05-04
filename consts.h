#pragma once
#include <stdio.h>
#include <stdlib.h>

#define CONST_CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CHARSET_LEN 63
#define FILE_MAX_LEN 100
#define INPUT_MAX_LEN 10
#define HASHES_PER_KERNEL 20000
#define BLOCKS 1
#define MAX_THREADS_PER_BLOCK 256

void ERR(const char * s) {
    fprintf(stderr,"%s\n",s);
    exit(EXIT_FAILURE);
}

const int KB = 1024;
const int MB = 1024 * 1024;
const int MAX_BLOCKS_PER_GRID = 65535;
