# CHERI-RISCV Release 0.001

## Cloning the repos

```sh
git clone https://github.com/CTSRD-CHERI/cheri-riscv-releases
cd cheri-riscv-releases
git checkout cheri_riscv_snapshot_20190603
git submodule update --init --recursive
```

## Getting started with CHERI Piccolo

To build CHERI Piccolo, you need to have a working Bluespec compiler installed.

From the `Piccolo/` directory, you can run the following commands to build the simulator and run the RISC-V ISA test suite:

```sh
cd Tests/elf_to_hex
make
cd ../../builds/RV64IUxCHERI_Piccolo_bluesim/
make compile simulator isa_tests
```

The simulator can be run with the following command:

```sh
./exe_HW_sim  +v1  +tohost
```

It expects a file named `Mem.hex` with the code to be run. Piccolo provides an `elf_to_hex` script in the `Tests/elf_to_hex/` directory to turn an `ELF` file into a  `Mem.hex` and can be used with the following command:

```sh
elf_to_hex  prog.elf  Mem.hex
```

## Building LLVM

```sh
cd llvm-project/llvm
mkdir build
cd build
cmake .. -G Ninja -DBUILD_SHARED_LIBS=ON -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=RISCV -DLLVM_ENABLE_PROJECTS="llvm;clang;lld"
ninja
```

## Building the example

```sh
cd examples
clang -target riscv32-unknown-freebsd -march=rv32ixcheri -mabi=il32pc64 -o cap-mode-memcpy cap-mode-memcpy.c -nostdlib -nostartfiles -fno-inline -ffreestanding -fuse-ld=lld -O2 -Wl,-Ttext-segment=0x80001000
```

Remove `-mabi=il32pc64` for pure capability code.
