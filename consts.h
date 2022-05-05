#pragma once
#include <stdio.h>
#include <stdlib.h>

#define CONST_CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CHARSET_LEN 63
#define FILE_MAX_LEN 10000
#define INPUT_MAX_LEN 10
#define HASHES_PER_KERNEL 4500
#define BLOCKS 5
#define MAX_BLOCKS 1000
#define MAX_THREADS_PER_BLOCK 800
#define MAX_THREADS_PER_BLOCK_2 800
#define INT_DIGITS 16

void ERR(const char * s) {
    fprintf(stderr,"%s\n",s);
    exit(EXIT_FAILURE);
}

const int KB = 1024;
const int MB = 1024 * 1024;
const int MAX_BLOCKS_PER_GRID = 65535;
