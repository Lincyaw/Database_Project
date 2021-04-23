//
// Created by llincyaw on 2021/4/22.
//

#include "linearSearch.h"

static int outputIndex = 3000;

void select(Buffer *buf, int valueC, int valueD, int startBlock, int endBlock) {
    unsigned char *blk = NULL;
    unsigned char *target = NULL;
    int ptr = 0;
    for (int idx = startBlock; idx <= endBlock; idx++) {
        // 读一个块
        if ((blk = readBlockFromDisk(idx, buf)) == NULL) {
            perror("Reading Block Failed!\n");
            return;
        }
        //一个blk存7个元组
        for (int i = 0; i < 7; i++) {
            int X = -2, Y = -2;
            getAttribute(blk, i, &X, &Y);

            if (X == -2 || Y == -2) {
                perror("没有取到数");
            }

            // C 是否有效
            if (valueC != -1) {
                if (valueD != -1) {
                    // 筛选两个
                    if (valueC == X && valueD == Y) {
                        // 维护一个指针, 指向target block, 作用是 write the tuple to the block
                        // target 为空，则在内存中申请一个 block
                        if (target == NULL) {
                            target = getNewBlockInBuffer(buf);
                        }
                        // 申请到了，就做
                        if (target != NULL) {
                            writeAttribute(target, ptr, X, Y);
                            ptr++;
                            // ptr == 7 表示这个block写满了
                            if (ptr == 7) {
                                writeBlockToDisk(target, outputIndex++, buf);
                                ptr = 0;
                                target = NULL;
                            }
                        }
                    }
                } else {
                    // 筛选 C
                    if (valueC == X) {
#ifdef DEBUG
                        printf("X: %d, Y: %d\n", X, Y);
#endif
                        // 维护一个指针, 指向target block, 作用是 write the tuple to the block
                        // target 为空，则在内存中申请一个 block
                        if (target == NULL) {
                            target = getNewBlockInBuffer(buf);
                        }
                        // 申请到了，就做
                        if (target != NULL) {
                            writeAttribute(target, ptr, X, Y);
                            ptr++;
                            // ptr == 7 表示这个block写满了
                            if (ptr == 7) {
                                writeBlockToDisk(target, outputIndex++, buf);
                                ptr = 0;
                                target = NULL;
                            }
                        }
                    }
                }
            } else {
                if (valueD != -1) {
                    // 筛选D
                    if (valueD == Y) {
                        // 维护一个指针, 指向target block, 作用是 write the tuple to the block
                        // target 为空，则在内存中申请一个 block
                        if (target == NULL) {
                            target = getNewBlockInBuffer(buf);
                        }
                        // 申请到了，就做
                        if (target != NULL) {
                            writeAttribute(target, ptr, X, Y);
                            ptr++;
                            // ptr == 7 表示这个block写满了
                            if (ptr == 7) {
                                writeBlockToDisk(target, outputIndex++, buf);
                                ptr = 0;
                                target = NULL;
                            }
                        }
                    }
                } else {
                    // 都不筛选
                }
            }

        }
        freeBlockInBuffer(blk, buf);
    }
}

int sortRelation(Buffer *buf, int startBlock, int endBlock) {
    unsigned char *blk = NULL;
    // 内排序
    for (int idx = startBlock; idx <= endBlock; idx++) {
        // 读一个块
        if ((blk = readBlockFromDisk(idx, buf)) == NULL) {
            perror("Reading Block Failed!\n");
            return -1;
        }
        unsigned char *innerSortBuffer = NULL;
        innerSortBuffer = getNewBlockInBuffer(buf);
        mySort(innerSortBuffer);
        freeBlockInBuffer(blk, buf);
        freeBlockInBuffer(innerSortBuffer, buf);
    }
    // 归并排序
    int num = endBlock - startBlock + 1;
    // 有序的块数
    int orderDistance = 1;


}