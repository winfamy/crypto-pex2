#pragma once
#include <stdint.h>
#include <math.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "hash_utils.h"
#include "md5.h"

__global__ void run_tinyhash_crack(unsigned char *);
__device__ __host__ void idxToString(uint64_t idx, unsigned char * copiedContent);

uint64_t g_resultA;
unsigned int g_fileContentLength;
unsigned char g_cracked[5][11];
unsigned char g_charset[CHARSET_LEN];
unsigned char g_targetHash[6];
unsigned char g_exampleHash[6];

__device__ unsigned char g_deviceExampleHash[6];
__device__ uint64_t g_deviceA;
__device__ unsigned char g_deviceTarget[6];
__device__ char g_deviceCharset[CHARSET_LEN];
__device__ unsigned char g_deviceCracked[5][11];
__device__ unsigned int g_deviceFileContentLength;
__shared__ int found;

__global__ void run_tinyhash_crack(unsigned char * fileContent) {
    found = 0;
    uint64_t idx = (threadIdx.x) * HASHES_PER_KERNEL;

    if (found >= 5) return;

    unsigned char threadStr[INPUT_MAX_LEN + 1];
    
    __shared__ unsigned char sharedTarget[6];
    __shared__ unsigned char sharedCracked[5][11];
    __shared__ unsigned char sharedCharset[CHARSET_LEN];

    memcpy(sharedTarget, g_deviceTarget, sizeof(unsigned char) * 6);
    memcpy(sharedCharset, g_deviceCharset, sizeof(char) * (CHARSET_LEN));
    memcpy(sharedCracked, g_deviceCracked, sizeof(unsigned char) * 5 * 11);

    uint32_t a, b, c, d;
    for (uint64_t i = 0; i < HASHES_PER_KERNEL; i++) {
        idxToString(idx, threadStr);
    
        uint32_t add_len = (unsigned int)g_strlen((const char *) threadStr);
        unsigned char * new_content = append_content(fileContent, threadStr, g_deviceFileContentLength, add_len);

        uint32_t data_len = add_len + g_deviceFileContentLength;
        // a = 1221;
        md5(new_content, data_len, &a, &b, &c, &d);

        unsigned char * hash = aToTinyHash(a);

        if (tinyHashCompare(sharedTarget, hash)) {
            memcpy(sharedCracked[found], threadStr, INPUT_MAX_LEN + 1);
            if (found <= 5) {
                found++;
            }
        }

        free(hash);
        free(new_content);

        idx++;
        memcpy(&g_deviceA, &found, sizeof(uint32_t));
        if (found >= 5) {
            break;
        }
    }

    memcpy(g_deviceCracked, sharedCracked, sizeof(unsigned char) * 5 * 11);
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
