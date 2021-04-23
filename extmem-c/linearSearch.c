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

int sortRelation(Buffer *buf, int startBlock, int endBlock) {
//    unsigned char *blk = NULL;
//    // 一共要排序多少个 blk
//    int num = endBlock - startBlock + 1;
//    // 划分为多少个子集
//    int nSet = num / (buf->numAllBlk - 2) + 1;
//
//
//    // 内排序
//    int blkCnt = 0;
//    for (int idx = startBlock; idx <= endBlock; idx++) {
//        // 读 1个块
//        blk = readBlockFromDisk(idx, buf);
//        if (blk == NULL) {
//            perror("Reading Block Failed!\n");
//            return -1;
//        }
//        unsigned char *innerSortBuffer = NULL;
//        innerSortBuffer = getNewBlockInBuffer(buf);
//        mySort(innerSortBuffer);
//        writeBlockToDisk(blk,idx,buf);
//        freeBlockInBuffer(blk, buf);
//        freeBlockInBuffer(innerSortBuffer, buf);
//    }
//
//    // 归并排序
//    int orderDistance = 2;
//    unsigned char *blk1 = NULL, *blk2 = NULL;
//    while (orderDistance < num) {
//        // orderDistance个一组的归并
//        for (int idx = 0; idx < num / orderDistance; idx++) {
//            /**
//             * 一共有 num/orderDistance 组
//             * 下面对组内进行排序
//             * 这个组的起始块是 startBlock + idx * orderDistance
//             * 结束块是 startBlock + (idx+1) * orderDistance - 1
//             * 其中还要对这个组分成两个部分，然后两者归并
//             */
//            for (int i = startBlock + idx * orderDistance;
//                 i < startBlock + idx * orderDistance + orderDistance / 2; i++) {
//                int a = i, b = i + orderDistance / 2;
//#ifdef DEBUG
//                printf("blk%d, blk%d\n", a, b);
//#endif
////                if ((blk1 = readBlockFromDisk(a, buf)) == NULL) {
////                    perror("Reading Block Failed!\n");
////                    return -1;
////                }
////                if ((blk2 = readBlockFromDisk(b, buf)) == NULL) {
////                    perror("Reading Block Failed!\n");
////                    return -1;
////                }
////                mergeTwoBlocks(blk1, blk2);
//            }
//            printf("<<<------------------->>>\n");
//        }
//        orderDistance *= 2;
//    }
#if DEBUG == BUBBLE_VERSION
    unsigned char *blk = NULL;
    // 内排序
    for (int idx = startBlock; idx <= endBlock; idx++) {
        blk = readBlockFromDisk(idx, buf);
        if (blk == NULL) {
            perror("Reading Block Failed!\n");
            return -1;
        }
        mySort(blk);
        writeBlockToDisk(blk, idx, buf);
        freeBlockInBuffer(blk, buf);
    }

    unsigned char *blk1 = NULL, *blk2 = NULL;
    for(int i = startBlock; i<=endBlock-1; i++){
        for(int j = i+1; j<=endBlock;j++){
            blk1 = readBlockFromDisk(i, buf);
            if (blk1 == NULL) {
                perror("Reading Block Failed!\n");
                return -1;
            }
            blk2 = readBlockFromDisk(j, buf);
            if (blk2 == NULL) {
                perror("Reading Block Failed!\n");
                return -1;
            }
            mergeTwoBlocks(blk1,blk2);

            writeBlockToDisk(blk1,i,buf);
            writeBlockToDisk(blk2,j,buf);
            freeBlockInBuffer(blk1,buf);
            freeBlockInBuffer(blk2,buf);
        }
    }
#endif

}