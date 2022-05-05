# Grady's PEX2

### Compilation Instructions
I suggest building this on WSL, as that is the only thing I can confirm it works on. 
A GTX 3070Ti was used for testing.

1. [Install CUDA Toolkit](https://docs.nvidia.com/cuda/wsl-user-guide/index.html)
2. Install libssl-dev
    1. `sudo apt install libssl-dev`
3. Compile project
    1. `nvcc main.cu -lcrypto -lssl` 
4. Run `a.out`!
5. (Optional) Run `test_hashes.sh` to confirm results.
    1. `source test_hashes.sh`

### Code
Must of the interesting stuff is in [main.cu](main.cu), [task1_kernel.h](task1_kernel.h), and [task2_kernel.h](task2_kernel.h).
For reading on how CUDA works, read https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html.

### Screenshots
![A test run](/screenshots/run.png "Project sample run")
![File hash confirmations](/screenshots/file_hashes.png "Output file hashes")

### Documentation
This is also contained within [main.cu](main.cu)
* Used https://github.com/iryont/md5-cracker for a basis on how to approach the problem and how to work with a CUDA device.
* Used https://opensource.apple.com/source/cvs/cvs-19/cvs/lib/md5.c for their implementation of MD5. Minor edits were made to how buffers were handled.

