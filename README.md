# CHERI-RISCV Release 0.01

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
cd llvm-project
mkdir build
cd build
cmake ../llvm -G Ninja -DBUILD_SHARED_LIBS=ON -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=RISCV -DLLVM_ENABLE_PROJECTS="llvm;clang;lld"
ninja
```

The built compiler binaries are in bin/.
For convenience, we also add some simlinks.

```sh
cd bin
ln -s clang riscv32-unknown-elf-clang
ln -s ld.lld riscv32-unknown-elf-ld
```

For even more convenience, you can add the bin directory to your $PATH.

## Building the example

Assuming the compiler built in the previous step is in $PATH.

```sh
cd examples
clang -target riscv32-unknown-freebsd -march=rv32ixcheri -mabi=il32pc64 -o cap-mode-memcpy cap-mode-memcpy.c -nostdlib -nostartfiles -fno-inline -ffreestanding -fuse-ld=lld -O2 -Wl,-Ttext-segment=0x80001000
```

This builds the example in pure capability mode.

## Building FreeRTOS/newlib

Assuming the compiler executables are in $PATH.

### Building newlib

The Galois FreeRTOS depends on newlib.
Only the libc part has been ported to purecap CHERI-RISCV, so this step only builds the libc.a but not everything in newlib.

```sh
cd newlib/newlib/libc
mkdir build
cd build
CC=riscv32-unknown-elf-clang CFLAGS="-march=rv32imxcheri -mabi=il32pc64 -ffreestanding -Werror -I$PWD/../include --sysroot=$SPAREFS/gfe/sysroot32" LD=riscv32-unknown-elf-ld LDFLAGS="-fuse-ld=lld -mno-relax" RANLIB=llvm-ranlib AR=llvm-ar ../configure --build=riscv32-unknown-elf --prefix=$SPAREFS/gfe/sysroot32 --disable-newlib-io-float
make
```

The output is a libc.a file in the build directory.

### Building purecap FreeRTOS

If you have installed newlib libc include directory and the built libc.a to the compiler's sysroot, then just follow these steps:

```sh
cd FreeRTOS-mirror/FreeRTOS/Demo/RISC-V_Galois_p1
make
```

This builds the `main_blinkly.elf` and the disassembly `main_blinky.dump`.

However, in case you cannot install the newlib headers and the libc.a to sysroot or the compiler fails to find them, modify the Makefile like this before typing make:

```
diff --git a/FreeRTOS/Demo/RISC-V_Galois_P1/Makefile b/FreeRTOS/Demo/RISC-V_Galois_P1/Makefile
index 3b625066b..70bdfb98d 100644
--- a/FreeRTOS/Demo/RISC-V_Galois_P1/Makefile
+++ b/FreeRTOS/Demo/RISC-V_Galois_P1/Makefile
@@ -109,7 +109,8 @@ INCLUDES = \
        -I./bsp/xilinx/spi \
        -I$(FREERTOS_SOURCE_DIR)/include \
        -I../Common/include \
-       -I$(FREERTOS_SOURCE_DIR)/portable/GCC/RISC-V
+       -I$(FREERTOS_SOURCE_DIR)/portable/GCC/RISC-V \
+       -I../../../../newlib/newlib/libc/include

 ifeq ($(PROG),main_blinky)
        CFLAGS += -DmainDEMO_TYPE=1
@@ -239,6 +240,7 @@ OBJS = $(CRT0_OBJ) $(PORT_ASM_OBJ) $(PORT_OBJ) $(RTOS_OBJ) $(DEMO_OBJ) $(APP_OBJ

 LDFLAGS = -target $(TARGET) -fuse-ld=lld -mno-relax
 LDFLAGS += -T link.ld -nostartfiles -nostdlib -defsym=_STACK_SIZE=4K
+LDFLAGS += -L../../../../newlib/newlib/libc/build
 LIBS    =  -lc

 $(info ASFLAGS=$(ASFLAGS))
```
