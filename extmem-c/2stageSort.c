//
// Created by llincyaw on 2021/4/25.
//

#include "2stageSort.h"


int TPMMS(Buffer *buf, int startBlock, int endBlock) {
    printf("-------------------------------------------------\n");
    printf("开始对 block%d 到 block%d 进行二阶段7路归并算法\n", startBlock, endBlock);
    printf("-------------------------------------------------\n");
#if RELEASE == TPMMS_VERSION
    bufferInfo bufCtl;

    // 内排序
    innerSort(buf, &bufCtl, startBlock, endBlock);

    bufCtl.totalBlocks = endBlock - startBlock + 1;
    bufCtl.commonSize = (int) buf->numAllBlk - 1;

    if (bufCtl.totalBlocks % bufCtl.commonSize == 0) {
        bufCtl.nSet = bufCtl.totalBlocks / bufCtl.commonSize;
        bufCtl.lastSize = bufCtl.commonSize;
    } else {
        bufCtl.nSet = bufCtl.totalBlocks / bufCtl.commonSize + 1;
        bufCtl.lastSize = bufCtl.totalBlocks % bufCtl.commonSize;
    }
    // 初始化
    for (int i = 0; i < 8; i++) {
        bufCtl.blkCnt[i] = 0;
    }
    int writeIndex = 1000;
    while (1) {
        // 归并
        int idx = 0;
        for (int group = 0; group < bufCtl.nSet; group++) {
            // 最后一组不足 commonSize个块
#if DEBUG == TPMMS_VERSION
            printf(">>>>>>>>>> GROUP: %d Begins\n", group + 1);
#endif
            int tmp = getSize(&bufCtl, group);
            int sep = 1;
            while (tmp > 7) {
                sep *= 7;
                tmp = ceil(tmp / 7.0);
            }
            // 读取这个组的所有的块，把指针存下来
            for (int i = 0; i < tmp; i++) {
#if DEBUG == TPMMS_VERSION
                printf("read head block %d\n", writeIndex + startBlock + i*sep + group * bufCtl.commonSize);
#endif
                // 每组的间隔都是 bufCtl.commonSize ， i作为组内的下标
                bufCtl.blkPtrs[i] = readBlockFromDisk(writeIndex + startBlock + i * sep + group * bufCtl.commonSize,
                                                      buf);
                if (bufCtl.blkPtrs[i] == NULL) {
                    printf("Reading Block Failed! Group: %d, block: %d\n", group, i);
                    return -1;
                }
            }
            // 获得缓冲区的块
            bufCtl.blkPtrs[7] = getNewBlockInBuffer(buf);
//todo maybe 因为下标不对，导致失败
            tuple ret;
            ret.finish = 0;
            bufCtl.curGroup = group;
            while (!ret.finish) {
                ret = getMin(&bufCtl, buf);
#if DEBUG == TPMMS_VERSION
                //            cnt++;
                //            printf(">> ");
                //            for(int i = 0; i<8; i++){
                //                printf("%d ", bufCtl.blkCnt[i]);
                //            }
                //            printf("\n");
                //            printf("> %d\tX: %d, Y: %d\n", cnt, ret.X, ret.Y);
#endif
                if (ret.finish)
                    break;
                writeAttribute(bufCtl.blkPtrs[7], bufCtl.blkCnt[7], ret.X, ret.Y);
                bufCtl.blkCnt[7]++;
                if (bufCtl.blkCnt[7] == 7) {
                    // 输出缓冲区满了，就把他输出
#if DEBUG == TPMMS_VERSION
                    printBlk(bufCtl.blkPtrs[7]);
                    printf("\n");
#endif
                    // 指定下一块
                    if ((idx + 1) % 7 == 0 || (idx + 1) == bufCtl.totalBlocks)
                        writeNext(bufCtl.blkPtrs[7], 0);
                    else
                        writeNext(bufCtl.blkPtrs[7], idx + startBlock + writeIndex + 1000 + 1);

                    writeBlockToDisk(bufCtl.blkPtrs[7], idx + startBlock + writeIndex + 1000, buf);
                    bufCtl.blkCnt[7] = 0;
                    // 清空输出缓冲区
                    clearBlock(bufCtl.blkPtrs[7]);
                    idx++;
                }
            }
            for (int i = 0; i < 8; i++) {
                if (bufCtl.blkPtrs[i] != NULL)
                    freeBlockInBuffer(bufCtl.blkPtrs[i], buf);
                bufCtl.blkCnt[i] = 0;
                bufCtl.blkPtrs[i] = NULL;
            }
#if DEBUG == TPMMS_VERSION
            printf(">>>>>>>>>>>>>>>>>>>>> GROUP: %d Finished\n", group + 1);
#endif
        }
        writeIndex += 1000;
        // 如果只有一组，那么经过上面的归并，应该就结束了
        if (bufCtl.nSet == 1) {
            break;
        }
        // 第n次归并，规模增加
        bufCtl.commonSize *= 7;
        if (bufCtl.totalBlocks % bufCtl.commonSize == 0) {
            bufCtl.nSet = bufCtl.totalBlocks / bufCtl.commonSize;
            bufCtl.lastSize = bufCtl.commonSize;
        } else {
            bufCtl.nSet = bufCtl.totalBlocks / bufCtl.commonSize + 1;
            bufCtl.lastSize = bufCtl.totalBlocks % bufCtl.commonSize;
        }
    }
#endif

#if RELEASE == BUBBLE_VERSION
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
    for (int i = 0; i < 8; i++) {
        if (bufCtl.blkPtrs[i] != NULL)
            freeBlockInBuffer(bufCtl.blkPtrs[i], buf);
        bufCtl.blkCnt[i] = 0;
        bufCtl.blkPtrs[i] = NULL;
    }
    return 0;
}

typedef struct same {
    int startBlock;
    int startTuple;
} Same;

int sortMergeJoin(Buffer *buf, int RStart, int REnd, int SStart, int SEnd) {
    printf("-------------------------------------------------\n");
    printf("开始对 block%d~block%d 与 block%d~block%d基于排序的连接操作算法\n", RStart, REnd, SStart, SEnd);
    printf("-------------------------------------------------\n");
    sort(buf, RStart, REnd);
    sort(buf, SStart, SEnd);
    bufferInfo bufCtl;
    Same sLog;
    int blkn = SStart + 3000;
    sLog.startBlock = SStart + 3000;
    sLog.startTuple = 0;
    bufCtl.blkPtrs[0] = readBlockFromDisk(RStart + 3000, buf);
    bufCtl.blkPtrs[1] = readBlockFromDisk(SStart + 3000, buf);
    RStart++;
    SStart++;
    bufCtl.blkPtrs[2] = getNewBlockInBuffer(buf);

    bufCtl.blkCnt[0] = 0;
    bufCtl.blkCnt[1] = 0;
    bufCtl.blkCnt[2] = 0;
    int cnt = 1;
    int tmp = 0;
    int valid = 0;
    while (RStart <= REnd + 1 && SStart <= SEnd + 1) {
        int Ra = -1, Rb = -1, Sc = -1, Sd = -1;
        getAttribute(bufCtl.blkPtrs[0], bufCtl.blkCnt[0], &Ra, &Rb);
        getAttribute(bufCtl.blkPtrs[1], bufCtl.blkCnt[1], &Sc, &Sd);

        if (Ra < Sc) {
            if (valid) {
                freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
                bufCtl.blkPtrs[1] = NULL;
                bufCtl.blkPtrs[1] = readBlockFromDisk(sLog.startBlock, buf);
                bufCtl.blkCnt[1] = sLog.startTuple;
                SStart = sLog.startBlock - 3000+1;
            }
            bufCtl.blkCnt[0]++;
            if (bufCtl.blkCnt[0] == 7) {
                freeBlockInBuffer(bufCtl.blkPtrs[0], buf);
                bufCtl.blkPtrs[0] = NULL;
                bufCtl.blkPtrs[0] = readBlockFromDisk(RStart + 3000, buf);
                RStart++;
                bufCtl.blkCnt[0] = 0;
            }
        } else if (Ra > Sc) {
            if (valid) {
                valid = 0;
            }
            bufCtl.blkCnt[1]++;
            if (bufCtl.blkCnt[1] == 7) {
                freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
                bufCtl.blkPtrs[1] = NULL;
                bufCtl.blkPtrs[1] = readBlockFromDisk(SStart + 3000, buf);
                blkn = SStart + 3000;
                SStart++;
                bufCtl.blkCnt[1] = 0;
            }
        } else {
            if (!valid) {
                // 存下 s 开始的地方
                sLog.startBlock = blkn;
                sLog.startTuple = bufCtl.blkCnt[1];
#ifdef  DEBUG
                printf("记录了位置 %d, %d\n", sLog.startBlock, sLog.startTuple);
#endif
                valid = 1;
            }
            bufCtl.blkCnt[1]++;
            if (bufCtl.blkCnt[1] == 7) {
                freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
                bufCtl.blkPtrs[1] = NULL;
                bufCtl.blkPtrs[1] = readBlockFromDisk(SStart + 3000, buf);
                blkn = SStart + 3000;
                SStart++;
                bufCtl.blkCnt[1] = 0;
            }

//            printf("%d: Ra = %d, Rb = %d, Sc = %d, Sd = %d\n", tmp++, Ra, Rb, Sc, Sd);
            writeAttribute(bufCtl.blkPtrs[2], bufCtl.blkCnt[2]++, Ra, Rb);
            if (bufCtl.blkCnt[2] == 7) {
                printf("结果写入磁盘%d\n", 5000 + cnt);
                writeBlockToDisk(bufCtl.blkPtrs[2], 5000 + cnt, buf);
                freeBlockInBuffer(bufCtl.blkPtrs[2], buf);
                bufCtl.blkPtrs[2] = NULL;
                bufCtl.blkPtrs[2] = getNewBlockInBuffer(buf);
                bufCtl.blkCnt[2] = 0;
                cnt++;
            }
            writeAttribute(bufCtl.blkPtrs[2], bufCtl.blkCnt[2]++, Sc, Sd);
            if (bufCtl.blkCnt[2] == 7) {
                writeBlockToDisk(bufCtl.blkPtrs[2], 5000 + cnt, buf);
                printf("结果写入磁盘%d\n", 5000 + cnt);
                freeBlockInBuffer(bufCtl.blkPtrs[2], buf);
                bufCtl.blkPtrs[2] = NULL;
                bufCtl.blkPtrs[2] = getNewBlockInBuffer(buf);
                bufCtl.blkCnt[2] = 0;
                cnt++;
            }
        }
    }
    if (bufCtl.blkCnt[2] != 0) {
        printf("结果写入磁盘%d\n", 5000 + cnt);
        writeBlockToDisk(bufCtl.blkPtrs[2], 5000 + cnt, buf);
        freeBlockInBuffer(bufCtl.blkPtrs[2], buf);
    }
    freeBlockInBuffer(bufCtl.blkPtrs[0], buf);
    freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
    return 0;
}

int intersection(Buffer *buf, int RStart, int REnd, int SStart, int SEnd) {
    printf("-------------------------------------------------\n");
    printf("开始对 block%d~block%d 与 block%d~block%d进行求交集\n", RStart, REnd, SStart, SEnd);
    printf("-------------------------------------------------\n");
//    sort(buf, RStart, REnd);
//    sort(buf, SStart, SEnd);
    bufferInfo bufCtl;
    Same sLog;
    int blkn = SStart + 3000;
    sLog.startBlock = SStart + 3000;
    sLog.startTuple = 0;

    bufCtl.blkPtrs[0] = readBlockFromDisk(RStart + 3000, buf);
    bufCtl.blkPtrs[1] = readBlockFromDisk(SStart + 3000, buf);
    RStart++;
    SStart++;
    bufCtl.blkPtrs[2] = getNewBlockInBuffer(buf);

    bufCtl.blkCnt[0] = 0;
    bufCtl.blkCnt[1] = 0;
    bufCtl.blkCnt[2] = 0;
    int cnt = 1;
    int tmp = 1;
    int valid = 0;

    while (RStart <= REnd + 1 && SStart <= SEnd + 1) {
        int Ra = -1, Rb = -1, Sc = -1, Sd = -1;
        getAttribute(bufCtl.blkPtrs[0], bufCtl.blkCnt[0], &Ra, &Rb);
        getAttribute(bufCtl.blkPtrs[1], bufCtl.blkCnt[1], &Sc, &Sd);
#ifdef DEBUG
        printf(">>>--- Ra = %d, Rb = %d, Sc = %d, Sd = %d\n", Ra, Rb, Sc, Sd);
#endif
        if (Ra < Sc) {
            if (valid) {
#ifdef DEBUG
                printf("切换到初始位,%d, %d\n", sLog.startBlock, sLog.startTuple);
#endif
                freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
                bufCtl.blkPtrs[1] = NULL;
                bufCtl.blkPtrs[1] = readBlockFromDisk(sLog.startBlock, buf);
                bufCtl.blkCnt[1] = sLog.startTuple;
                SStart = sLog.startBlock - 3000+1;
            }
            bufCtl.blkCnt[0]++;
            if (bufCtl.blkCnt[0] == 7) {
                freeBlockInBuffer(bufCtl.blkPtrs[0], buf);
                bufCtl.blkPtrs[0] = NULL;
                bufCtl.blkPtrs[0] = readBlockFromDisk(RStart + 3000, buf);
                RStart++;
                bufCtl.blkCnt[0] = 0;
            }
        } else if (Ra > Sc) {
            if (valid) {
                valid = 0;
            }
            bufCtl.blkCnt[1]++;
            if (bufCtl.blkCnt[1] == 7) {
                freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
                bufCtl.blkPtrs[1] = NULL;
                bufCtl.blkPtrs[1] = readBlockFromDisk(SStart + 3000, buf);
                blkn = SStart + 3000;
                SStart++;
                bufCtl.blkCnt[1] = 0;
            }
        } else {
            if (!valid) {
                // 存下 s 开始的地方
                sLog.startBlock = blkn;
                sLog.startTuple = bufCtl.blkCnt[1];
#ifdef  DEBUG
                printf("记录了位置 %d, %d\n", sLog.startBlock, sLog.startTuple);
#endif
                valid = 1;
            }
            bufCtl.blkCnt[1]++;
            if (bufCtl.blkCnt[1] == 7) {
                freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
                bufCtl.blkPtrs[1] = NULL;

                bufCtl.blkPtrs[1] = readBlockFromDisk(SStart + 3000, buf);
                blkn = SStart + 3000;
                SStart++;
                bufCtl.blkCnt[1] = 0;
            }

            if (Rb != Sd) {
                continue;
            }
            printf("%d: Ra = %d, Rb = %d, Sc = %d, Sd = %d\n", tmp++, Ra, Rb, Sc, Sd);

            writeAttribute(bufCtl.blkPtrs[2], bufCtl.blkCnt[2]++, Ra, Rb);
            if (bufCtl.blkCnt[2] == 7) {
                printf("结果写入磁盘%d\n", 6000 + cnt);
                writeBlockToDisk(bufCtl.blkPtrs[2], 6000 + cnt, buf);
                freeBlockInBuffer(bufCtl.blkPtrs[2], buf);
                bufCtl.blkPtrs[2] = NULL;
                bufCtl.blkPtrs[2] = getNewBlockInBuffer(buf);
                bufCtl.blkCnt[2] = 0;
                cnt++;
            }
        }
    }

    if (bufCtl.blkCnt[2] != 0) {
        printf("结果写入磁盘%d\n", 6000 + cnt);
        writeBlockToDisk(bufCtl.blkPtrs[2], 6000 + cnt, buf);
        freeBlockInBuffer(bufCtl.blkPtrs[2], buf);
    }
    freeBlockInBuffer(bufCtl.blkPtrs[0], buf);
    freeBlockInBuffer(bufCtl.blkPtrs[1], buf);
    return 0;
}

