#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

#include "file_utils.h"
#include "md5_utils.h"
#include "md5_kernel.h"

#define CONST_CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

int main() {
    int devices;
    cudaGetDeviceCount(&devices);
    cudaSetDeviceFlags(cudaDeviceScheduleSpin);

    unsigned int content_len;
    unsigned char * file_content = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    get_file_contents("./files/samplefile.txt", file_content, &content_len);
    
    unsigned char * hash = (unsigned char *)malloc(64);
    unsigned int hash_len;
    
    hash_len = md5(file_content, content_len, hash);
    print_hash(hash, hash_len);
}