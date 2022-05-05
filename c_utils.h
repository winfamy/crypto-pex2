#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <cuda_runtime.h>

#include "consts.h"
#include "hash_utils.h"

void get_file_contents(const char * filename, unsigned char * content, unsigned int * content_len);
__device__ __host__ size_t g_strlen(const char * str);

void get_file_contents(const char * filename, unsigned char * content, unsigned int * content_len) {
    FILE * f = fopen(filename, "rb");
    if (!f) {
        printf("[-] Failed to open file %s\n", filename);
        exit(1);
    }

    int bytes_read = fread(content, sizeof(char), FILE_MAX_LEN, f);
    *content_len = bytes_read;
}

__device__ __host__ void append_content(unsigned char * out, unsigned char * input_str, unsigned char * add_str, int len1, int len2) {
    memcpy(out, input_str, len1);
    memcpy(out + len1, add_str, len2 + 1);
}

__device__ __host__ size_t g_strlen(const char * str) {
	register const char *s;

	for (s = str; *s; ++s);
	return(s - str);
}

// Function to swap two numbers
__device__ __host__ void swap(unsigned char *x, unsigned char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
__device__ __host__ void reverse(unsigned char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
}
 
// Iterative function to implement `itoa()` function in C
__device__ __host__ void itoa(int value, unsigned char * buffer, int base)
{
    // consider the absolute value of the number
    int n = value;
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    reverse(buffer, 0, i - 1);
}