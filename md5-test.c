#include <stdio.h>
#include <string.h>
#include "md5_utils.h"

int main()
{
    EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    hash_len = md5("foobar", 6, hash);

    for(unsigned int i=0; i<hash_len; i++)
        printf("%02x", hash[i]);
    printf("\n");

    return 0;
}