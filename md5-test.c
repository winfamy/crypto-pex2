#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "md5.h"
#include "hash_utils.h"
#include "consts.h"
#include "c_utils.h"
// #include "md5_kernel.h"

int main()
{
    uint32_t target = 100000;
    unsigned char output[FILE_MAX_LEN];
    unsigned char targetHash[16];
    unsigned char stringHash[16];
    unsigned char string[] = "gRv8";
    unsigned char filename[256];
    printf("=== BEGIN TASK 2 ===\n");
    printf("Input contract filename:  ");
    scanf("%s", filename);
    printf("\n");

    unsigned int content_len;
    unsigned char * file_content = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    unsigned char * firstPart = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    get_file_contents("files/contract.txt", file_content, &content_len);
    sscanf((const char *) file_content, "%[A-Za-z:. $]%d", firstPart, &target);

    md5((const char *)file_content, content_len, targetHash);

    printf("File MD5 Hash: ");
    print_hash(targetHash);
    printf("\nFile TinyHash Hash: ");
    print_tinyhash(targetHash);
    printf("\n\n");

    md5((const char *)string, 4, stringHash);

    printf("String MD5 Hash: ");
    print_hash(stringHash);
    printf("\nString TinyHash Hash: ");
    print_tinyhash(stringHash);
    printf("\n\n");

    unsigned char numStr[20];
    uint32_t idx = 999999;
    itoa(idx, numStr, 10);
    printf("NumStr: %s\n", numStr);
    // unsigned char numStr[8];
    // uint32_t idx = 33529;
    // itoa_simple((const char *) numStr, idx);
    // printf("%s\n", numStr);
    // append_content(output, firstPart, numStr, strlen(firstPart), strlen(numStr));
    // printf("%s\n", output);

    // unsigned char calcHash[6];
    // md5(output, strlen(firstPart) + strlen(numStr), &a, &b, &c, &d);
    // aToTinyHash(calcHash, a);
    // printf("%s\n", calcHash);
}

// 00100010111101100101100000111000