# HDMI IN Demo for VIM4

**Note: Only test on Ubuntu 22.04.**

## Install Build Dependencies

```bash
$ sudo apt update
$ sudo apt install libboost-dev libgrpc++-dev libgrpc-dev libboost-all-dev protobuf-compiler protobuf-compiler-grpc  android-liblog-dev
$ sudo apt install hdmiin-demo
```

## Build

```bash
$ make -j8 all
```

## Releated Binaries

```
$ ls *.so hdmiin-demo tvservice
hdmiin-demo  libtvclient.so  libtv.so  tvservice
```

## Update

Copy `libtvclient.so` and `libtv.so` to `/usr/lib/aarch64-linux-gnu/`.
Copy `tvservice` and `hdmiin-demo` to `/usr/bin/`.
