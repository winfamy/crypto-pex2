#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <cuda_runtime.h>

__device__ __host__ unsigned char * aToTinyHash(uint32_t a) {
    unsigned char * str = (unsigned char *)malloc(sizeof(unsigned char) * 6);
    memset(str, 0, 6);

    int decnum, rem, j = 0;
    for (int i = 0; i < 3; i++) {
        decnum = (a >> (8 * i)) & 0xFF;
        while (decnum != 0)
        {
            rem = decnum % 16;
            if(rem < 10)
                rem = rem + 48;
            else
                rem = rem + 55;
            
            if (j % 2 == 0) {
                str[j+1] = rem;
            } else {
                str[j-1] = rem;
            }

            j++;
            decnum = decnum / 16;
        }
    }

    str[5] = '\0';
    return str;
}

__device__ __host__ bool tinyHashCompare(unsigned char * a, unsigned char * b) {
    return (a[0] == b[0]) &&
        (a[1] == b[1]) && 
        (a[2] == b[2]) && 
        (a[3] == b[3]) && 
        (a[4] == b[4]);
}