# tmp

## about

Temp repo for simple game server

## build

```sh
mkdir build
cd build
conan install .. --build=http_parser
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DDEBUG=[1;0] -DTESTS=[1;0]
make
```

