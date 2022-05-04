#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

#include "c_utils.h"
#include "hash_utils.h"
#include "md5_kernel.h"
#include "md5.h"
#include "cuda_utils.cu"

int main() {
    ERROR_CHECK(cudaSetDevice(0));
    ERROR_CHECK(cudaSetDeviceFlags(cudaDeviceScheduleSpin));

    unsigned int content_len;
    unsigned char * file_content = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    get_file_contents("./files/samplefile.txt", file_content, &content_len);
    
    uint32_t a, b, c, d;
    md5(file_content, content_len, &a, &b, &c, &d);
    unsigned char * targetHash = aToTinyHash(a);
    printf("Welcome \n");
    printf("Target: %s\n", targetHash);

    unsigned char * fileContent; 
    unsigned char * copiedContent; 
    cudaMalloc(&fileContent, sizeof(unsigned char) * FILE_MAX_LEN);
    cudaMalloc(&copiedContent, sizeof(unsigned char) * (FILE_MAX_LEN + INPUT_MAX_LEN));
    cudaMemcpy(fileContent, file_content, content_len, cudaMemcpyHostToDevice);
    
    memset(g_cracked, 0, sizeof(unsigned char) * 5 * 11);
    memcpy(g_charset, CONST_CHARSET, sizeof(unsigned char) * CHARSET_LEN);
    memcpy(g_targetHash, targetHash, sizeof(unsigned char) * 6);
    memcpy(&g_fileContentLength, &content_len, sizeof(unsigned int));

    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceTarget, g_targetHash, sizeof(unsigned char) * 6, 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceCharset, g_charset, sizeof(unsigned char) * CHARSET_LEN, 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceCracked, g_cracked, sizeof(unsigned char) * 5 * 11, 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceFileContentLength, &g_fileContentLength, sizeof(unsigned int), 0));

    cudaEvent_t clockBegin;
    cudaEvent_t clockLast;
    cudaEventCreate(&clockBegin);
    cudaEventCreate(&clockLast);
    cudaEventRecord(clockBegin, 0);

    int bytes = 98304;
    ERROR_CHECK(cudaFuncSetAttribute(run_tinyhash_crack, cudaFuncAttributeMaxDynamicSharedMemorySize, bytes));
    
    run_tinyhash_crack<<<BLOCKS, MAX_THREADS_PER_BLOCK>>>(fileContent);
    ERROR_CHECK(cudaDeviceSynchronize());


    ERROR_CHECK(cudaMemcpyFromSymbol(&g_resultA, g_deviceA, sizeof(uint64_t), 0, cudaMemcpyDeviceToHost));
    ERROR_CHECK(cudaMemcpyFromSymbol(g_cracked, g_deviceCracked, sizeof(unsigned char) * 5 * 11, 0, cudaMemcpyDeviceToHost));
    ERROR_CHECK(cudaMemcpyFromSymbol(g_exampleHash, g_deviceExampleHash, sizeof(unsigned char) * 6, 0, cudaMemcpyDeviceToHost));
    cudaFree(fileContent);

    printf("%lu\n", g_resultA);
    for (int i = 0; i < 5; i++) {
        printf("%s\n", g_cracked[i]);
    }
}