#pragma once
#include <stdlib.h>
#include <stdio.h>

#define FILE_MAX_LEN 65535

void get_file_contents(const char * filename, unsigned char * content, unsigned int * content_len);
void get_file_contents(const char * filename, unsigned char * content, unsigned int * content_len) {
    FILE * f = fopen(filename, "rb");
    int bytes_read = fread(content, sizeof(char), FILE_MAX_LEN, f);
    *content_len = bytes_read;
}