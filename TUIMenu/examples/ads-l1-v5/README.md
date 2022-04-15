```sh
mkdir build
cd build
conan install ..
cmake .. -DADS_L1_V5=1 # -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build .
```
