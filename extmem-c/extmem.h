/*
 * extmem.h
 * Zhaonian Zou
 * Harbin Institute of Technology
 * Jun 22, 2011
 */

#ifndef EXTMEM_H
#define EXTMEM_H

#define BLOCK_AVAILABLE 0
#define BLOCK_UNAVAILABLE 1
#define RELATION_R_BEGIN 1
#define RELATION_R_END 16
#define RELATION_S_BEGIN 17
#define RELATION_S_END 48
#define BUBBLE_VERSION 0
#define TPMMS_VERSION 1
#define RELEASE TPMMS_VERSION
//#define DEBUG TPMMS_VERSION
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct tagBuffer {
    unsigned long numIO; /* Number of IO's*/
    size_t bufSize; /* Buffer size*/
    size_t blkSize; /* Block size */
    size_t numAllBlk; /* Number of blocks that can be kept in the buffer */
    size_t numFreeBlk; /* Number of available blocks in the buffer */
    unsigned char *data; /* Starting address of the buffer */
} Buffer;

struct writeBlk {
    unsigned char *blk;
    unsigned int size;
    unsigned int blkNum; //预计写回的块地址
};


typedef struct bufferController {
    // 一共要排序多少个块
    int totalBlocks;
    // 每个子集几个块
    int commonSize;
    // 最后一个子集几个块
    int lastSize;
    // 几个子集
    int nSet;
    // 8个blk指针
    unsigned char *blkPtrs[8];
    // 8个buffer块的计数
    int blkCnt[8];
    // 现在在哪组
    int curGroup;
} bufferInfo;

typedef struct tuples {
    int blockIdx;
    int X;
    int Y;
    int finish;
}tuple;

int getSize(bufferInfo* bufCtl, int idx);
/* Initialize a buffer with the specified buffer size and block size.
 * If the initialization fails, the return value is NULL;
 * otherwise the pointer to the buffer.
 */
Buffer *initBuffer(size_t bufSize, size_t blkSize, Buffer *buf);

/* Free the memory used by a buffer. */
void freeBuffer(Buffer *buf);

/* Apply for a new block from a buffer.
 * If no free blocks are available in the buffer,
 * then the return value is NULL; otherwise the pointer to the block.
 */
unsigned char *getNewBlockInBuffer(Buffer *buf);

/* Set a block in a buffer to be available. */
void freeBlockInBuffer(unsigned char *blk, Buffer *buf);

/* Drop a block on the disk */
int dropBlockOnDisk(unsigned int addr);

/* Read a block from the hard disk to the buffer by the address of the block. */
unsigned char *readBlockFromDisk(unsigned int addr, Buffer *buf);

/* Read a block in the buffer to the hard disk by the address of the block. */
int writeBlockToDisk(unsigned char *blkPtr, unsigned int addr, Buffer *buf);

/**
 * 在对应的blk里读数据
 * @param blk
 * @param tupleId 元组号
 * @param x
 * @param y
 */
void getAttribute(unsigned char *blk, int tupleId, int *x, int *y);

/**
 * 获得下一个blk
 * @param blk
 * @return
 */
int getNext(unsigned char *blk);

void writeNext(unsigned char *blk,int addr);

/**
 * 向对应的blk里写数据，不涉及磁盘
 * @param blk
 * @param tupleId 元组id
 * @param x
 * @param y
 */
void writeAttribute(unsigned char *blk, int tupleId, int x, int y);

// 用于内存，内排序
void mySort(unsigned char *blk);

/**
 * 原地归并
 * @param blk1 排序后的较小块
 * @param blk2 排序后的较大块
 */
void mergeTwoBlocks(unsigned char *blk1, unsigned char *blk2);


void printBlk(unsigned char *blk);
/**
 * 在每一组的block中找到最小的那个数，并且把对应的那个组的指针+1
 * @param bufCtl
 * @return
 */
tuple getMin(bufferInfo* bufCtl);


tuple getMinV2(bufferInfo *bufCtl, Buffer* buf);
#endif // EXTMEM_H
