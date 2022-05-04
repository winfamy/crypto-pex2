#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "consts.h"
#include "hash_utils.h"

void get_file_contents(const char * filename, unsigned char * content, unsigned int * content_len);
__device__ __host__ size_t g_strlen(const char * str);

void get_file_contents(const char * filename, unsigned char * content, unsigned int * content_len) {
    FILE * f = fopen(filename, "rb");
    int bytes_read = fread(content, sizeof(char), FILE_MAX_LEN, f);
    *content_len = bytes_read;
}

__device__ __host__ unsigned char * append_content(unsigned char * input_str, unsigned char * add_str, int len1, int len2) {
    unsigned char * new_content = (unsigned char *)malloc(sizeof(unsigned char) * (len1 + len2));

    memcpy(new_content, input_str, len1);
    memcpy(new_content + len1, add_str, len2);
    return new_content;
}

__device__ __host__ size_t g_strlen(const char * str) {
	register const char *s;

	for (s = str; *s; ++s);
	return(s - str);
}