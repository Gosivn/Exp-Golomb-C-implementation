//
//  bs.h
//  Exp_Golomb
//
//  Created by Jinmmer on 2018/4/1.
//  Copyright © 2018年 Jinmmer. All rights reserved.
//

#ifndef bs_h
#define bs_h

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    uint8_t* start; // 指向buf头部指针
    uint8_t* p;     // 当前指针位置
    uint8_t* end;   // 指向buf尾部指针
    int bits_left;  // 当前读取字节的剩余(可用/未读)比特个数
} bs_t;

bs_t* bs_init(bs_t* b, uint8_t* buf, size_t size);
bs_t* bs_new(uint8_t* buf, size_t size);
void bs_free(bs_t* b);

void bs_write_u1(bs_t* b, uint32_t v);
void bs_write_u(bs_t* b, int n, uint32_t v);
uint32_t bs_read_u1(bs_t* b);
uint32_t bs_read_u(bs_t* b, int n);

/**指数哥伦布编码*/
void bs_write_ue( bs_t *b, unsigned int val);
void bs_write_se(bs_t* b, int32_t v);
void bs_write_te( bs_t *b, int x, int val);

/**解码*/
uint32_t bs_read_ue(bs_t* b);
int32_t  bs_read_se(bs_t* b);
uint32_t bs_read_te( bs_t *b, int x );

#endif /* bs_h */
