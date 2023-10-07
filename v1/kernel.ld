ENTRY(boot) /* エントリポイントの設定 */

SECTIONS {
    . = 0x80200000; /* ベースアドレスの設定 */

    .text : { /* コード領域 */
        KEEP(*(.text.boot)); /* .text.bootセクションを先頭にする */
        *(.text .text.*);
    }

    .rodata : ALIGN(4) { /* 定数データ領域 読み取り専用 */
        *(.rodata .rodata.*);
    }

    .data : ALIGN(4) { /* 読み書き可能データ領域 */
        *(.data .data.*);
    }

    .bss : ALIGN(4) { /* 読み書き可能データ領域 .dataと違い、初期値がゼロの変数を配置する */
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(4);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;

    . = ALIGN(4096); /* 4KB境界に配置 */
    __free_ram = .; /* 動的割り当て可能なデータ領域 */
    . += 64 * 1024 * 1024; /* 64MB */
    __free_ram_end = .;
}
