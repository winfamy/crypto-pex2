#include <stdint.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda_utils.h"

__global__ void run_tinyhash_crack(char * charset);
__global__ void run_tinyhash_crack(char * charset) {
    uint32_t idx = (blockIdx.x * blockDim.x + threadIdx.x) * HASHES_PER_KERNEL;
}