/*
 * extmem.c
 * Zhaonian Zou
 * Harbin Institute of Technology
 * Jun 22, 2011
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"

Buffer *initBuffer(size_t bufSize, size_t blkSize, Buffer *buf) {
    int i;

    buf->numIO = 0;
    buf->bufSize = bufSize;
    buf->blkSize = blkSize;
    buf->numAllBlk = bufSize / (blkSize + 1);
    buf->numFreeBlk = buf->numAllBlk;
    buf->data = (unsigned char *) malloc(bufSize * sizeof(unsigned char));

    if (!buf->data) {
        perror("Buffer Initialization Failed!\n");
        return NULL;
    }

    memset(buf->data, 0, bufSize * sizeof(unsigned char));
    return buf;
}

void freeBuffer(Buffer *buf) {
    free(buf->data);
}

unsigned char *getNewBlockInBuffer(Buffer *buf) {
    unsigned char *blkPtr;

    if (buf->numFreeBlk == 0) {
        perror("Buffer is full!\n");
        return NULL;
    }

    blkPtr = buf->data;

    while (blkPtr < buf->data + (buf->blkSize + 1) * buf->numAllBlk) {
        if (*blkPtr == BLOCK_AVAILABLE)
            break;
        else
            blkPtr += buf->blkSize + 1;
    }

    *blkPtr = BLOCK_UNAVAILABLE;
    buf->numFreeBlk--;
    return blkPtr + 1;
}

void freeBlockInBuffer(unsigned char *blk, Buffer *buf) {
    *(blk - 1) = BLOCK_AVAILABLE;
    buf->numFreeBlk++;
}

int dropBlockOnDisk(unsigned int addr) {
    char filename[40];

    sprintf(filename, "../data/%d.blk", addr);

    if (remove(filename) == -1) {
        perror("Dropping Block Fails!\n");
        return -1;
    }

    return 0;
}

unsigned char *readBlockFromDisk(unsigned int addr, Buffer *buf) {
    char filename[40];
    unsigned char *blkPtr, *bytePtr;
    char ch;

    if (buf->numFreeBlk == 0) {
        perror("Buffer Overflows!\n");
        return NULL;
    }

    blkPtr = buf->data;

    while (blkPtr < buf->data + (buf->blkSize + 1) * buf->numAllBlk) {
        if (*blkPtr == BLOCK_AVAILABLE)
            break;
        else
            blkPtr += buf->blkSize + 1;
    }

    sprintf(filename, "../data/%d.blk", addr);
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        perror("Reading Block Failed!\n");
        return NULL;
    }

    *blkPtr = BLOCK_UNAVAILABLE;
    blkPtr++;
    bytePtr = blkPtr;

    while (bytePtr < blkPtr + buf->blkSize) {
        ch = fgetc(fp);
        *bytePtr = ch;
        bytePtr++;
    }

    fclose(fp);
    buf->numFreeBlk--;
    buf->numIO++;
    return blkPtr;
}

int writeBlockToDisk(unsigned char *blkPtr, unsigned int addr, Buffer *buf) {
    char filename[40];
    unsigned char *bytePtr;

    sprintf(filename, "../data/%d.blk", addr);
    FILE *fp = fopen(filename, "w");

    if (!fp) {
        perror("Writing Block Failed!\n");
        return -1;
    }

    for (bytePtr = blkPtr; bytePtr < blkPtr + buf->blkSize; bytePtr++)
        fputc((int) (*bytePtr), fp);

    fclose(fp);
    *(blkPtr - 1) = BLOCK_AVAILABLE;//重新将块置为可用，特别要注意这里
    buf->numFreeBlk++;
    buf->numIO++;
    return 0;
}

void getAttribute(unsigned char *blk, int tupleId, int *x, int *y) {
    char str[5];
    int i = tupleId;
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + k);
    }

    int X = atoi(str);
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + i * 8 + 4 + k);
    }
    int Y = atoi(str);
    *x = X;
    *y = Y;
}

int getNext(unsigned char *blk, int tupleId) {
    char str[5];
    for (int k = 0; k < 4; k++) {
        str[k] = *(blk + tupleId * 8 + k);
    }
    int addr = atoi(str);
    return addr;
}
void writeNext(unsigned char *blk, int tupleId, int addr) {
    char str[5];
    sprintf(str,"%d",addr);
    for (int k = 0; k < 4; k++) {
        *(blk + tupleId * 8 + k) = str[k];
    }
}

void writeAttribute(unsigned char *blk, int tupleId, int x, int y) {
    char str[5];
    int i = tupleId;

    sprintf(str, "%d", x);
    for (int k = 0; k < 4; k++) {
        *(blk + i * 8 + k) = str[k];
    }
    sprintf(str, "%d", y);
    for (int k = 0; k < 4; k++) {
        *(blk + i * 8 + 4 + k) = str[k];
    }
}

void mySort(unsigned char *blk) {
    for (int i = 0; i < 7 - 1; i++) {
        for (int j = 0; j < 7 - 1 - i; j++) {
            int X1 = -2, Y1 = -2;
            getAttribute(blk, j, &X1, &Y1);
            int X2 = -2, Y2 = -2;
            getAttribute(blk, j + 1, &X2, &Y2);
            if (X1 > X2) {
                writeAttribute(blk, j, X2, Y2);
                writeAttribute(blk, j + 1, X1, Y1);
            }
        }
    }
}
static void swap(int *a, int*b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
void mergeTwoBlocks(unsigned char *blk1, unsigned char *blk2) {
    int i = 0, j = 0;
    while (i < 7) {
        int X1 = -2, Y1 = -2;
        getAttribute(blk1, i, &X1, &Y1);
        int X2 = -2, Y2 = -2;
        getAttribute(blk2, j, &X2, &Y2);
        if(X1<=X2){
            // 小于，则加一
            i++;
        }else{
            // 大于,就交换，然后重新排序 blk2
            swap(&X1,&X2);
            swap(&Y1,&Y2);
            writeAttribute(blk1, i, X1,Y1);
            writeAttribute(blk2, j, X2,Y2);
            mySort(blk2);
        }
    }
    // 此时，blk1 <= blk2
}

void printBlk(unsigned char *blk)
{
    int X = -1;
    int Y = -1;
    for (int j = 0; j < 7; j++)
    {
        getAttribute(blk, j, &X, &Y);
        printf("(X=%d, Y=%d) \n", X, Y);
    }
}