#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <cuda_runtime.h>

__device__ __host__ bool tinyHashCompare(unsigned char * a, unsigned char * b) {
    return (a[0] == b[0] && a[1] == b[1] && (a[2] & 0xF0) == (b[2] & 0xF0));
}

__device__ __host__ void hexify(const unsigned char* input, char* output) {
    const char* map = "0123456789abcdef";

    for(int i = 0; i < 16; i++) {
        output[i*2] = map[(input[i] & 0xF0) >> 4];
        output[i*2+1] = map[(input[i] & 0x0F)];
    }
    output[32] = '\0';
}

void print_hash(unsigned char * result) {
    char output[32];
    hexify(result, output);
    printf("%s", output);
}

void print_tinyhash(unsigned char * result) {
    char output[32];
    hexify(result, output);
    output[5] = '\0';
    printf("%s", output);
}