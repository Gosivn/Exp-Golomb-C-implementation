//
//  bs.c
//  Exp_Golomb
//
//  Created by Jinmmer on 2018/4/1.
//  Copyright © 2018年 Jinmmer. All rights reserved.
//

#include "bs.h"

bs_t* bs_init(bs_t* b, uint8_t* buf, size_t size)
{
    b->start = buf;  // 指向buf起始位置
    b->p = buf;      // 初始位置与start保持一致
    b->end = buf + size;    // 指向buf末尾
    b->bits_left = 8;   // 默认剩余8比特可用
    return b;
}

bs_t* bs_new(uint8_t* buf, size_t size)
{
    bs_t* b = (bs_t*)malloc(sizeof(bs_t));
    bs_init(b, buf, size);
    return b;
}

void bs_free(bs_t* b)
{
    free(b);
}

/** 是否已读到末尾（end_of_file） */
int bs_eof(bs_t* b) { if (b->p >= b->end) { return 1; } else { return 0; } }

void bs_write_u1(bs_t* b, uint32_t v)
{
    // 1.剩余比特先减1
    b->bits_left--;
    
    if (! bs_eof(b))
    {
        // 2.见文章
        (*(b->p)) |= ((v & 0x01) << b->bits_left);
    }
    
    // 3.判断是否写到字节末尾，如果是指针位置移向下一字节，比特位初始为8
    if (b->bits_left == 0) { b->p ++; b->bits_left = 8; }
}


/**
 写入n个比特
 
 @param b 比特流操作句柄
 @param n 参数v所需的比特位个数
 @param v 待写入的值
 */
void bs_write_u(bs_t* b, int n, uint32_t v)
{
    int i;
    for (i = 0; i < n; i++)
    {
        // 循环调用bs_write_u1()，写入n个比特
        bs_write_u1(b, (v >> ( n - i - 1 ))&0x01 );
    }
}

/**
 ue(v) 无符号指数哥伦布编码
 */
void bs_write_ue( bs_t *b, unsigned int val)
{
    // val + 1所需的比特个数
    int i_size = 0;
    // 1.值为0~255时，所需的比特位个数表
    static const int i_size0_255[256] =
    {
        1,      // 0的二进制所需的比特个数
        1,      // 1的二进制所需的比特个数
        2,2,    // 2~3的二进制所需的比特个数
        3,3,3,3,    // 4~7的二进制所需的比特个数
        4,4,4,4,4,4,4,4,  // 8~15的二进制所需的比特个数
        5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,  // 16~31的二进制所需的比特个数
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,  // 32~63的二进制所需的比特个数
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  // 64~127的二进制所需的比特个数
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,  // 128~255的二进制所需的比特个数
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    };
    
    if( val == 0 ) // 输入为0，直接编码为1
    {
        bs_write_u1( b, 1 );
    }
    else
    {
        // 1.指数哥伦布编码第一步，先把输入值+1
        unsigned int tmp = ++val;
        // 2.判断所需比特个数是否大于16位
        if( tmp >= 0x00010000 )
        {
            i_size += 16;
            tmp >>= 16;
        }
        // 3.判断此时所需比特个数是否大于8位
        if( tmp >= 0x100 )
        {
            i_size += 8;
            tmp >>= 8;
        }
        // 4.最终tmp移位至8位以内，去查表
        i_size += i_size0_255[tmp];
        // 5.最终得出编码val所需的总比特数：2 * i_size - 1
        // 写入Buffer
        bs_write_u( b, 2 * i_size - 1, val );
    }
}

/**
 se(v) 有符号指数哥伦布编码
 */
void bs_write_se(bs_t* b, int32_t v)
{
    if (v <= 0)
    {
        bs_write_ue(b, -v*2);
    }
    else
    {
        bs_write_ue(b, v*2 - 1);
    }
}

/**
 te(v) 截断指数哥伦布编码
 */
void bs_write_te( bs_t *b, int x, int val)
{
    if( x == 1 )
    {
        bs_write_u1( b, 1&~val );
    }
    else if( x > 1 )
    {
        bs_write_ue( b, val );
    }
}
