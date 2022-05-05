#pragma once
#include <stdint.h>
#include <math.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "hash_utils.h"
#include "md5.h"

__global__ void run_task1_crack();
__device__ __host__ void idxToString(uint64_t idx, unsigned char * copiedContent);

uint64_t g_resultA;
unsigned char g_targetHash[16];
unsigned char g_cracked[5][11];
unsigned char g_charset[CHARSET_LEN];

__device__ unsigned char g_deviceTarget[16];
__device__ char g_deviceCharset[CHARSET_LEN];
__device__ unsigned char g_deviceCracked[5][11];

__global__ void run_task1_crack() {
    uint64_t idx = (blockDim.x * blockIdx.x + threadIdx.x) * HASHES_PER_KERNEL;

    unsigned char hashStr[16];
    unsigned char threadStr[INPUT_MAX_LEN + 1];
    __shared__ int found;
    found = 0;

    __shared__ unsigned char sharedTarget[16];
    __shared__ unsigned char sharedCracked[11];
    __shared__ unsigned char sharedCharset[CHARSET_LEN];

    memset(sharedCracked, 0, sizeof(unsigned char) * 11);
    memcpy(sharedTarget, g_deviceTarget, sizeof(unsigned char) * 16);
    memcpy(sharedCharset, g_deviceCharset, sizeof(char) * (CHARSET_LEN));

    for (uint32_t i = 0; i < HASHES_PER_KERNEL; i++) {
        __syncthreads();
        if (found) return;

        idxToString(idx, threadStr);
    
        uint32_t str_len = (unsigned int)g_strlen((const char *) threadStr);
        md5(threadStr, str_len, hashStr);
        if (tinyHashCompare(sharedTarget, hashStr)) {
            found = 1;
            memcpy(g_deviceCracked[blockIdx.x], threadStr, sizeof(unsigned char) * 11);
            return;
        }

        idx++;
    }
}

__device__ __host__ void idxToString(uint64_t idx, unsigned char * str) {
    memset(str, 0, INPUT_MAX_LEN);
    if (idx == 0) {
        str[0] = '0';
        return;
    }

    int start_index = -1;
    for (int i = INPUT_MAX_LEN; i >= 0; i--) {
        uint64_t charsetSize_powi = (uint64_t)pow(CHARSET_LEN - 1, i);
        uint32_t amt_in_idx = (uint32_t)floorf(idx / charsetSize_powi);
        if (amt_in_idx >= 1) {
            if (start_index == -1) start_index = i;

            str[start_index - i] = CONST_CHARSET[amt_in_idx];
            idx -= amt_in_idx * charsetSize_powi;
        } else {
            if (str[0] != 0) {
                str[i - start_index] = CONST_CHARSET[0];
            }
        }
    }

    return;
}
