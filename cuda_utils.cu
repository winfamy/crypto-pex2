#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "consts.h"

#include <stdio.h>
#include <cmath>
#include <iostream>

#define ERROR_CHECK(X) { gpuAssert((X), __FILE__, __LINE__); }

inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true){
  if(code != cudaSuccess){
    std::cout << "Error: " << cudaGetErrorString(code) << " " << file << " " << line << std::endl;
    if(abort){
      exit(code);
    }
  }
}