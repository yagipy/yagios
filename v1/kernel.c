#include "kernel.h"

#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[], __bss_end[],
    __stack_top[];  // リンカスクリプト内で定義されている各シンボルを宣言

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
  // 指定したレジスタに値を入れる
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  // CPUの実行モードがS-ModeからM-Modeに切り替わり、OpenSBIの処理ハンドラが呼び出される
  __asm__ __volatile__("ecall"
                       : "=r"(a0), "=r"(a1)
                       : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                         "r"(a6), "r"(a7)
                       : "memory");

  return (struct sbiret){.error = a0, .value = a1};
}

// https://github.com/riscv-non-isa/riscv-sbi-doc/blob/master/src/ext-legacy.adoc#extension-console-putchar-eid-0x01
void putchar(char ch) { sbi_call(ch, 0, 0, 0, 0, 0, 0, 1); }

__attribute__((naked))
__attribute__((aligned(4))) // stvecは例外ハンドラのアドレスだけでなく下位2ビットにモードを表すフラグを持っているため、関数の先頭アドレスを4バイト境界にアラインする
void kernel_entry(void) {
    __asm__ __volatile__(
        // 浮動小数点レジスタはカーネル内で使われないので、ここでは保存する必要がない
        // 一般的にスレッドの切り替え時に保存・退避が行われる
        "csrw sscratch, sp\n" // 例外発生時のスタックポインタ(sp)をsscratchに保存
        "addi sp, sp, -4 * 31\n"
        "sw ra, 4 * 0(sp)\n"
        "sw gp, 4 * 1(sp)\n"
        "sw tp, 4 * 2(sp)\n"
        "sw t0, 4 * 3(sp)\n"
        "sw t1, 4 * 4(sp)\n"
        "sw t2, 4 * 5(sp)\n"
        "sw s3, 4 * 6(sp)\n"
        "sw s4, 4 * 7(sp)\n"
        "sw s5, 4 * 8(sp)\n"
        "sw s6, 4 * 9(sp)\n"
        "sw a0, 4 * 10(sp)\n"
        "sw a1, 4 * 11(sp)\n"
        "sw a2, 4 * 12(sp)\n"
        "sw a3, 4 * 13(sp)\n"
        "sw a4, 4 * 14(sp)\n"
        "sw a5, 4 * 15(sp)\n"
        "sw a6, 4 * 16(sp)\n"
        "sw a7, 4 * 17(sp)\n"
        "sw s0, 4 * 18(sp)\n"
        "sw s1, 4 * 19(sp)\n"
        "sw s2, 4 * 20(sp)\n"
        "sw s3, 4 * 21(sp)\n"
        "sw s4, 4 * 22(sp)\n"
        "sw s5, 4 * 23(sp)\n"
        "sw s6, 4 * 24(sp)\n"
        "sw s7, 4 * 25(sp)\n"
        "sw s8, 4 * 26(sp)\n"
        "sw s9, 4 * 27(sp)\n"
        "sw s10, 4 * 28(sp)\n"
        "sw s11, 4 * 29(sp)\n"

        "csrr a0, sscratch\n"
        "sw a0, 4 * 30(sp)\n"

        "mv a0, sp\n" // a0にspをセット
        "call handle_trap\n" // handle_trap関数を呼び出す

        "lw ra, 4 * 0(sp)\n"
        "lw gp, 4 * 1(sp)\n"
        "lw tp, 4 * 2(sp)\n"
        "lw t0, 4 * 3(sp)\n"
        "lw t1, 4 * 4(sp)\n"
        "lw t2, 4 * 5(sp)\n"
        "lw t3, 4 * 6(sp)\n"
        "lw t4, 4 * 7(sp)\n"
        "lw t5, 4 * 8(sp)\n"
        "lw t6, 4 * 9(sp)\n"
        "lw a0, 4 * 10(sp)\n"
        "lw a1, 4 * 11(sp)\n"
        "lw a2, 4 * 12(sp)\n"
        "lw a3, 4 * 13(sp)\n"
        "lw a4, 4 * 14(sp)\n"
        "lw a5, 4 * 15(sp)\n"
        "lw a6, 4 * 16(sp)\n"
        "lw a7, 4 * 17(sp)\n"
        "lw s0, 4 * 18(sp)\n"
        "lw s1, 4 * 19(sp)\n"
        "lw s2, 4 * 20(sp)\n"
        "lw s3, 4 * 21(sp)\n"
        "lw s4, 4 * 22(sp)\n"
        "lw s5, 4 * 23(sp)\n"
        "lw s6, 4 * 24(sp)\n"
        "lw s7, 4 * 25(sp)\n"
        "lw s8, 4 * 26(sp)\n"
        "lw s9, 4 * 27(sp)\n"
        "lw s10, 4 * 28(sp)\n"
        "lw s11, 4 * 29(sp)\n"
        "lw sp, 4 * 30(sp)\n"
        "sret\n"
    );
}

void handle_trap(struct trap_frame *f) {
    uint32_t scause = READ_CSR(scause);
    uint32_t stval = READ_CSR(stval);
    uint32_t user_pc = READ_CSR(sepc);

    PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}

void kernel_main(void) {
  const char *s = "\n\nHello World!\n";
  for (int i = 0; s[i] != '\0'; i++) {
    putchar(s[i]);
  }
  printf("\n\nHello %s!\n", "World");
  printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);
  WRITE_CSR(stvec, (uint32_t) kernel_entry);
  __asm__ __volatile__("unimp"); // 無効な命令

  for (;;) {
    __asm__ __volatile__("wfi");
  }
}

__attribute__((section(
    ".text.boot"))) __attribute__((naked)) // Function
                                           // prologueとepilogueを生成しない
                                           void
                                           boot(void) {
  __asm__ __volatile__(
      "mv sp, %[stack_top]\n"
      "j kernel_main\n"
      :
      : [stack_top] "r"(__stack_top));
}
