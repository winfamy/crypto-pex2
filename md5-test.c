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
    uint32_t a, b, c, d;

    unsigned char threadStr[INPUT_MAX_LEN + 1];
    unsigned int content_len;
    unsigned char * file_content = (unsigned char *)malloc(sizeof(char) * FILE_MAX_LEN);
    get_file_contents("./files/testfile.txt", file_content, &content_len);

    md5(file_content, &a, &b, &c, &d);
    unsigned char * hash = aToTinyHash(a);
    printf("%s\n", hash);

    int add_len = g_strlen(threadStr);
    printf("%s \n", file_content);
    unsigned char * new_string = append_content(file_content, threadStr, content_len, add_len);
    printf("%s \n", new_string);
    // printf("%s \n", file_content);

    return 0;
}

// 00100010111101100101100000111000