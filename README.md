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

### Screenshots
![A test run](/screenshots/run.png "Project sample run")
![File hash confirmations](/screenshots/file_hashes.png "Output file hashes")

