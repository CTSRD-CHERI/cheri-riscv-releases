# CHERI-RISCV Release 0.001

## Cloning the repos

```
git clone https://github.com/CTSRD-CHERI/cheri-riscv-releases
cd cheri-riscv-releases
git checkout cheri_riscv_snapshot_20190603
git submodule update --init --recursive
```

## Building LLVM

```
cd llvm-project/llvm
mkdir build
cd build
cmake .. -G Ninja -DBUILD_SHARED_LIBS=ON -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=RISCV -DLLVM_ENABLE_PROJECTS="llvm;clang;lld"
ninja
```

## Building the example

```
cd examples
clang -target riscv32-unknown-freebsd -march=rv32ixcheri -mabi=il32pc64 -o cap-mode-memcpy cap-mode-memcpy.c -nostdlib -nostartfiles -fno-inline -ffreestanding -fuse-ld=lld -O2 -Wl,-Ttext-segment=0x80001000
```

Remove `-mabi=il32pc64` for pure capability code.
