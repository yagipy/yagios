#include "common.h"

void putchar(char c);

void printf(const char *fmt, ...) {
  va_list vargs;
  va_start(vargs, fmt);

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
        case '\0':
          putchar('%');
          goto end;
        case '%':
          putchar('%');
          break;
        case 's': {  // 文字列
          const char *s = va_arg(vargs, const char *);
          while (*s) {
            putchar(*s);
            s++;
          }
          break;
        }
        case 'd': {  // 10進数
          int value = va_arg(vargs, int);
          if (value < 0) {
            putchar('-');
            value = -value;
          }

          int divisor = 1;
          while (value / divisor > 9) {
            divisor *= 10;
          }

          while (divisor > 0) {
            putchar('0' + value / divisor);
            value %= divisor;
            divisor /= 10;
          }

          break;
        }
        case 'x': {  // 16進数
          int value = va_arg(vargs, int);
          for (int i = 7; i >= 0; i--) {
            int nibble = (value >> (i * 4)) & 0xf;
            putchar("0123456789abcdef"[nibble]);
          }
        }
      }
    } else {
      putchar(*fmt);
    }

    fmt++;
  }

end:
  va_end(vargs);
}

// bufの先頭からnバイト分をcで埋める
void *memset(void *buf, char c, size_t n) {
    uint8_t *p = (uint8_t *)buf;
    while (n--) {
        *p++ = c;
    }
    return buf;
}

// srcからnバイト分をdstにコピー
void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *d = (uint8_t *) dst;
    const uint8_t *s = (const uint8_t *) src;
    while (n--) {
        *d++ = *s++;
    }
    return dst;
}

// srcの文字列をdstにコピー
// dstのメモリ領域よりsrcの方が長い時でも、dstのメモリ領域を越えてコピーを行うため、バグや脆弱性に繋がりやすい
char *strcpy(char *dst, const char *src) {
    char *d = dst;
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dst;
}

// s1とs2を比較
// s1とs2が等しい場合は0を、s1の方が大きい場合は正の値を、s2の方が大きい場合は負の値を返す
int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            break;
        }
        s1++;
        s2++;
    }

    // https://www.man7.org/linux/man-pages/man3/strcmp.3.html#:~:text=both%20interpreted%20as%20type%20unsigned%20char
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
