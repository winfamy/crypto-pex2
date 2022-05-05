// Documentation:
// Used https://github.com/iryont/md5-cracker for a basis on how to approach the problem and how to work with a CUDA device.
// Used https://opensource.apple.com/source/cvs/cvs-19/cvs/lib/md5.c for their implementation of MD5. Minor edits were made to how buffers were handled.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

#include "c_utils.h"
#include "hash_utils.h"
#include "task1_kernel.h"
#include "task2_kernel.h"
#include "md5.h"
#include "cuda_utils.cu"

void task1();
void task2();

int main() {
    ERROR_CHECK(cudaSetDevice(0));
    ERROR_CHECK(cudaSetDeviceFlags(cudaDeviceScheduleSpin));

    printf("CyS 431 PEX2 – Hash Collider - by C1C Grady Phillips\n");
    task1();
    task2();
}

void task1() {
    unsigned char targetHash[16];
    char filename[256];
    printf("=== BEGIN TASK 1 ===\n");
    printf("Input filename:  ");
    scanf("%s", filename);
    printf("\n");

    unsigned int content_len;
    unsigned char * file_content = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    get_file_contents((const char *)filename, file_content, &content_len);

    md5(file_content, content_len, targetHash);

    printf("File MD5 Hash: ");
    print_hash(targetHash);
    printf("\nFile TinyHash Hash: ");
    print_tinyhash(targetHash);
    printf("\n\n");
    
    printf("[ ] Setting up CUDA global memory...\n");
    memset(g_cracked, 0, sizeof(unsigned char) * 5 * 11);
    memcpy(g_charset, CONST_CHARSET, sizeof(unsigned char) * CHARSET_LEN);
    memcpy(g_targetHash, targetHash, sizeof(unsigned char) * 16);
    printf("[+] Set up global memory!\n");

    printf("[ ] Copying CUDA device memory...\n");
    unsigned char * cracked;
    cudaMalloc(&cracked, sizeof(unsigned char) * MAX_BLOCKS * 11);
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceTarget, g_targetHash, sizeof(unsigned char) * 16, 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceCharset, g_charset, sizeof(unsigned char) * CHARSET_LEN, 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceCracked, g_cracked, sizeof(unsigned char) * 5 * 11, 0));
    printf("[+] Set up device memory!\n");

    cudaEvent_t clockBegin;
    cudaEvent_t clockLast;
    cudaEventCreate(&clockBegin);
    cudaEventCreate(&clockLast);
    cudaEventRecord(clockBegin, 0);

    int bytes = 98304;
    ERROR_CHECK(cudaFuncSetAttribute(run_task1_crack, cudaFuncAttributeMaxDynamicSharedMemorySize, bytes));
    
    printf("\n[ ] Colliding hashes...\n");
    run_task1_crack<<<BLOCKS, MAX_THREADS_PER_BLOCK>>>();
    ERROR_CHECK(cudaDeviceSynchronize());
    ERROR_CHECK(cudaMemcpyFromSymbol(g_cracked, g_deviceCracked, sizeof(unsigned char) * BLOCKS * 11, 0, cudaMemcpyDeviceToHost));

    for (int i = 0; i < BLOCKS; i++) {
        printf("[+] Found collision: %s\n", g_cracked[i]);
        snprintf(filename, 255, "output/collision%d.txt", i);
        FILE * fp = fopen(filename, "w");
        if (fp) {
            fwrite(g_cracked[i], strlen((const char *) g_cracked[i]), 1, fp);
            printf("[+]\tWrote collision to file %s\n", filename);
            fclose(fp);
        } else {
            printf("[-]\tFailed to write collision to file %s.\n", filename);
        }
    }

    float milliseconds = 0;
    cudaEventRecord(clockLast, 0);
    cudaEventSynchronize(clockLast);
    cudaEventElapsedTime(&milliseconds, clockBegin, clockLast);
    
    std::cout << "[+] Ran task 1 in " << milliseconds << " ms" << std::endl << std::endl << std::endl;
}

void task2() {
    uint32_t target = 100000;
    unsigned char targetHash[6];
    unsigned char filename[256];
    printf("=== BEGIN TASK 2 ===\n");
    printf("Input contract filename:  ");
    scanf("%s", filename);
    printf("\n");

    unsigned int content_len;
    unsigned char * file_content = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    unsigned char * firstPart = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    get_file_contents((const char *)filename, file_content, &content_len);
    sscanf((const char *) file_content, "%[A-Za-z:. $]%d", firstPart, &target);

    md5(file_content, content_len, targetHash);
    uint16_t firstPart_len = strlen((const char *) firstPart);

    printf("File MD5 Hash: ");
    print_hash(targetHash);
    printf("\nFile TinyHash Hash: ");
    print_tinyhash(targetHash);
    printf("\n\n");
    
    printf("[ ] Setting up CUDA global memory...\n");
    memcpy(&g_targetNum, &target, sizeof(uint32_t));
    memcpy(g_targetHash, targetHash, sizeof(unsigned char) * 3);
    memcpy(&g_firstPartLength, &firstPart_len, sizeof(uint16_t));
    memcpy(g_firstPartContent, firstPart, sizeof(unsigned char) * firstPart_len);
    printf("[+] Set up global memory!\n");

    printf("[ ] Copying CUDA device memory...\n");
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceTarget2, g_targetHash, sizeof(unsigned char) * 3, 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceTargetNum, &g_targetNum, sizeof(uint32_t), 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceFirstPartLength, &g_firstPartLength, sizeof(uint16_t), 0));
    ERROR_CHECK(cudaMemcpyToSymbol(g_deviceFirstPartContent, g_firstPartContent, sizeof(unsigned char) * (g_firstPartLength + 1), 0));
    printf("[+] Set up device memory!\n");

    cudaEvent_t clockBegin;
    cudaEvent_t clockLast;
    cudaEventCreate(&clockBegin);
    cudaEventCreate(&clockLast);
    cudaEventRecord(clockBegin, 0);
    
    printf("\n[ ] Colliding hashes...\n");
    run_task2_crack<<<MAX_BLOCKS, MAX_THREADS_PER_BLOCK_2>>>();
    ERROR_CHECK(cudaDeviceSynchronize());
    ERROR_CHECK(cudaMemcpyFromSymbol(&g_resultNum, g_deviceNum, sizeof(uint32_t), 0, cudaMemcpyDeviceToHost));
    if (g_resultNum != 0) {
        printf("[+] Found collision with value $%d\n", g_resultNum);

        FILE * fp = fopen("output/newcontract.txt", "w");
        if (fp) {
            fwrite(firstPart, firstPart_len, 1, fp);
            fprintf(fp, "%d", g_resultNum);
            fclose(fp);

            printf("[+]\tWrote contract to output/newcontract.txt\n");
        } else {
            printf("[-]\tFailed to write contract to output/newcontract.txt\n");
        }
    } else {
        printf("[-] Failed to find collision with value $%d\n", target);
    }

    float milliseconds = 0;
    cudaEventRecord(clockLast, 0);
    cudaEventSynchronize(clockLast);
    cudaEventElapsedTime(&milliseconds, clockBegin, clockLast);
    
    std::cout << "[+] Ran task 2 in " << milliseconds << " ms" << std::endl;
}