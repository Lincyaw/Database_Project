//
// Created by llincyaw on 2021/4/22.
//

#include "linearSearch.h"

static int outputIndex = 3000;

void selectTable(Buffer *buf, int valueC, int valueD, int startBlock, int endBlock) {
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

void generateIndex(Buffer *buf, int startBlock, int endBlock) {
    unsigned char *index;
    index = getNewBlockInBuffer(buf);

    // key, tupleId
    unsigned char *blk;
    int flag = 1;
    int tuple = 0;
    int blockIndex = 0;
    for (int i = startBlock; i < endBlock; i++) {
        blk = readBlockFromDisk(i, buf);
        for (int j = 0; j < 7; j++) {
            int X = -1, Y = -1;
            getAttribute(blk, j, &X, &Y);
            if (X == -1) {
                continue;
            }
            if (X % 10 == 0) {
                if (flag) {
                    printf("%d,%d\n", X, Y);
                    writeAttribute(index, tuple++, X, Y);
                    flag = 0;
                    if (tuple == 7 || i == endBlock - 1) {
                        writeBlockToDisk(index, 4000 + blockIndex, buf);
                        freeBlockInBuffer(index, buf);
                        index = getNewBlockInBuffer(buf);
                        blockIndex++;
                        tuple = 0;
                    }
                }
            } else {
                flag = 1;
            }

        }
        freeBlockInBuffer(blk, buf);
    }
    writeBlockToDisk(index, 4000 + blockIndex, buf);
    freeBlockInBuffer(index, buf);
}