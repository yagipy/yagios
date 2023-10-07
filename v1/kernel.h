#pragma once

#include "common.h"

// __FILE__と__LINE__を正しく表示するためにマクロとして定義
// do-while: while(0)なので1回しか実行されない
//   複数の文からなるマクロを定義したいときに頻出する書き方
//   単に{...}で括ると、if文などと組み合わせた時に意図しない動作に繋がる
// ##__VA_ARGS__: 可変長引数を受け取るマクロを定義するときに便利なコンパイラ拡張
//   ##を付けることで、可変長引数が空の時に直前の,を削除する(引数が1つの時にもコンパイルが通るようになる)
#define PANIC(fmt, ...) \
    do {                \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while (1) {}    \
    } while (0)

#define READ_CSR(reg) ({ \
    unsigned long __tmp; \
    __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; \
})

#define WRITE_CSR(reg, val) \
    do { \
        uint32_t __tmp = (val); \
        __asm__ __volatile__("csrw " #reg ", %0" : : "r"(__tmp)); \
    } while (0)

struct sbiret {
  long error;
  long value;
};

// スタックに積まれている元の実行状態の構造
struct trap_frame {
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed));