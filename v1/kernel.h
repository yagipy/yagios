#pragma once

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

struct sbiret {
  long error;
  long value;
};
