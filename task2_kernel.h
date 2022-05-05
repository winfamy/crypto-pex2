#pragma once
#include <string>
#include <stdint.h>
#include <math.h>

#include "md5.h"
#include "consts.h"
#include "c_utils.h"
#include "hash_utils.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void run_task2_crack();

uint32_t g_resultNum;
uint32_t g_targetNum;
uint16_t g_firstPartLength;
unsigned char g_firstPartContent[FILE_MAX_LEN];

__device__ uint32_t g_deviceNum;
__device__ uint32_t g_deviceTargetNum;
__device__ uint16_t g_deviceFirstPartLength;
__device__ unsigned char g_deviceTarget2[3];
__device__ unsigned char g_deviceFirstPartContent[FILE_MAX_LEN];
__device__ unsigned char g_outNumStr[20];
__device__ unsigned char g_outTestStr[FILE_MAX_LEN];
__device__ unsigned char g_outHash[16];
__device__ uint32_t g_outAddedLen;


__global__ void run_task2_crack() {
    uint32_t idx = (blockDim.x * blockIdx.x + threadIdx.x);
    if (idx >= g_deviceTargetNum) return;

    // if (idx == 99000) {
    // memcpy(&g_deviceNum, &idx, sizeof(uint32_t));

    unsigned char hashStr[16];
    unsigned char testStr[FILE_MAX_LEN + 16];
    unsigned char numStr[20];
    
    __shared__ unsigned char sharedTarget[3];
    memcpy(sharedTarget, g_deviceTarget2, sizeof(unsigned char) * 3);

    itoa(idx, numStr, 10);
    int add_len = g_strlen((const char *) numStr);
    
    memcpy(testStr, g_deviceFirstPartContent, g_deviceFirstPartLength);
    memcpy(testStr + g_deviceFirstPartLength, numStr, add_len + 1);

    int test_str_len = g_strlen((const char *) testStr);
    md5(testStr, test_str_len, hashStr);
    
    if (tinyHashCompare(sharedTarget, hashStr)) {
        memcpy(&g_deviceNum, &idx, sizeof(uint32_t));
    }
}