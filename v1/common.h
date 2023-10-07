#pragma once

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short int16_t;
typedef unsigned int uint32_t;
typedef long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t; // 物理メモリアドレスを表す型
typedef uint32_t vaddr_t; // 仮想メモリアドレスを表す型で、uintptr_t相当。

#define true 1
#define false 0
#define NULL ((void *) 0)

// https://clang.llvm.org/docs/LanguageExtensions.html#variadic-function-builtins
#define align_up(value, align) __builtin_align_up(value, align) // valueをalignの倍数に切り上げる
#define is_aligned(value, align) __builtin_is_aligned(value, align)
#define offsetof(type, member) __builtin_offsetof(type, member) // 構造体のメンバが構造体の先頭から何バイト目にあるかを返す
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);
