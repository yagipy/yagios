# yagios v1
WIP

## Debug
```sh
# コンパイル&QEMUの起動
./run.sh

# QEMUモニタの起動
Ctrl+A C # QEMU起動後に入力
# QEMUモニタを終了したい場合は、QEMUモニタ上でqを入力

# レジスタダンプ
info registers # QEMUモニタ上で実行

# オブジェクトファイルのダンプ
llvm-objdump -d kernel.elf

# 各関数や変数のアドレスを表示
llvm-nm kernel.elf
```

## References
https://operating-system-in-1000-lines.vercel.app/ja/welcome
RISC-V SBI specification: https://github.com/riscv-non-isa/riscv-sbi-doc
OpenSBI: https://github.com/riscv-software-src/opensbi
