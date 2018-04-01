//
//  main.c
//  Exp_Golomb
//
//  Created by Jinmmer on 2018/4/1.
//  Copyright © 2018年 Jinmmer. All rights reserved.
//

#include <stdio.h>
#include "bs.h"

void print_binary_value(uint8_t n);

#define MAX_BUFFER_SIZE (20)

int main(int argc, const char * argv[]) {
    
    // 1.开辟缓冲区
    // 注意要使用calloc进行初始化，不要用malloc()，否则会出现部分值不为0
    uint8_t *buffer = (uint8_t *)calloc(1, MAX_BUFFER_SIZE);
    
    // 2.初始化比特流处理句柄
    bs_t *bs = bs_new(buffer, MAX_BUFFER_SIZE);
    
    printf("Hex code at start:\n");
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        printf("%02x", buffer[i]);
        printf("\t");
    }
    printf("\n");
    
    // 3.ue(v)
    bs_write_ue(bs, 0);
    bs_write_ue(bs, 1);
    bs_write_ue(bs, 2);
    bs_write_ue(bs, 3);
    bs_write_ue(bs, 4);
    bs_write_ue(bs, 5);
    bs_write_ue(bs, 6);
    bs_write_ue(bs, 7);
    bs_write_ue(bs, 8);
    bs_write_ue(bs, 9);
    // 4.se(v)
    bs_write_se(bs, 9);
    // 5.te(v)
    bs_write_te(bs, 9, 9);
    
    
    printf("Binary code at end:\n");
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        print_binary_value(buffer[i]);
        printf("\t");
    }
    printf("\n");
    
    printf("Hex code at end:\n");
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        printf("%02x", buffer[i]);
        printf("\t");
    }
    printf("\n");
    
    return 0;
}

/**打印二进制*/
void print_binary_value(uint8_t n) {
    int byte_n = sizeof(uint8_t);
    int bit_n = byte_n * 8;
    int mask;
    for (int j = 0; j < bit_n; j++) {
        mask = bit_n-1-j;
        putchar((n&(1<<mask)) == 0? '0':'1');
    }
}
