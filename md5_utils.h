#pragma once
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

void print_hash();
unsigned int md5(unsigned char * data, unsigned int data_len, unsigned char * md_digest);

void print_hash(unsigned char * hash, unsigned int hash_len) {
    for(int i = 0; i < hash_len; i++)
        printf("%02x", hash[i]);
    printf("\n");
}

unsigned int md5(unsigned char * data, unsigned int data_len, unsigned char * md_digest)
{
    OpenSSL_add_all_digests();
    const EVP_MD * md = EVP_get_digestbyname("MD5");
    if(!md) {
        printf("Unable to init MD5 digest\n");
        exit(1);
    }

    unsigned int hash_len;
    EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, data_len);
    EVP_DigestFinal_ex(mdctx, md_digest, &hash_len);
    EVP_MD_CTX_destroy(mdctx);
    return hash_len;
}