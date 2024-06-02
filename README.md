# LAB26
## Usage
### Prerequisites
You will need:
- llvm clang with google santizers
- cmake 3.25 installed
### Building
```bash
rm -rf build && mkdir build
cd build
cmake ..
make && make install
cd ..
```
## Running
```bash
./build/lab26
```
## Merge sort tests
```bash
cd build
ctest --verbose
```

