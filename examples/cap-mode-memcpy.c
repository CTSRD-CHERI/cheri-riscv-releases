#include <cheri_init_globals.h>

#define STACK_SIZE 1048576

#define STR(x) #x
#define XSTR(x) STR(x)

#if __riscv_xlen == 32
typedef unsigned int size_t;
#elif __riscv_xlen == 64
typedef unsigned long size_t;
#else
#error Unknown __riscv_xlen value
#endif

__asm__(
".text\n"
".option push\n"
".option nocapmode\n"
".global _start\n"
"_start:\n"
"       lla     sp, stack\n"
"       cfromptr        csp, ddc, sp\n"
"       li      t0, "XSTR(STACK_SIZE)"\n"
"       csetbounds      csp, csp, t0\n"
"       cincoffset      csp, csp, t0\n"
"       lla     t0, _start_purecap\n"
"       cfromptr        ct0, ddc, t0\n"
"       li      t1, 1\n"
"       csetflags       ct0, ct0, t1\n"
"       cjr     ct0\n"
".option pop\n"
"\n"
".section \".tohost\",\"aw\",@progbits\n"
".align 6\n"
".globl tohost\n"
"tohost:\n"
"       .dword 0\n"
".align 6\n"
".globl fromhost\n"
"fromhost:\n"
"       .dword 0\n"
);

int x;
int y;
int *px;
int *py;
int **ppy = &py;
int **ppy_1 = &py+1;
__uintcap_t cap42 = 42;

__attribute__((aligned(4096))) char stack[STACK_SIZE];

void *memcpy(void *dst, const void *src, size_t len) {
        for (int i = 0; i < len; ++i)
                ((char *)dst)[i] = ((char *)src)[i];
        return dst;
}

void _start_purecap(void) {
        cheri_init_globals_3(__builtin_cheri_global_data_get(),
                __builtin_cheri_program_counter_get(),
                __builtin_cheri_global_data_get());
        x = 42;
        y++;
        px = &x;
        py = &y;
        __asm__ __volatile__("" ::: "memory");
        memcpy(py, px, sizeof(x));
        y++;
        (**ppy)++;
        cap42++;
}
