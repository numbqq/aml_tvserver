# HDMI IN Demo for VIM4

**Note: Only test on Ubuntu 22.04.**

## Install Build Dependencies

```bash
$ sudo apt update
$ sudo apt install libboost-dev libgrpc++-dev libgrpc-dev libboost-all-dev protobuf-compiler protobuf-compiler-grpc  android-liblog-dev
$ sudo dpkg -i hdmiin-demo_0.1-202205_arm64.deb
```

## Build

```bash
$ make -j8 all
```
