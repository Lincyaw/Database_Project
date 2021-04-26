//
// Created by llincyaw on 2021/4/25.
//

#include "2stageSort.h"
int TPMMS1(Buffer *buf, int startBlock, int endBlock) {
#if RELEASE == TPMMS_VERSION
    bufferInfo bufCtl;
    // 一共要排序多少个 blk
    bufCtl.totalBlocks = endBlock - startBlock + 1;
    // 划分为多少个子集
    bufCtl.totalBlocks = endBlock - startBlock + 1;

    // 内排序
    for (int idx = startBlock; idx <= endBlock; idx++) {
        // 读 1个块
        bufCtl.blkPtrs[0] = readBlockFromDisk(idx, buf);
        if (bufCtl.blkPtrs[0] == NULL) {
            perror("Reading Block Failed!\n");
            return -1;
        }
        mySort(bufCtl.blkPtrs[0]);
        writeBlockToDisk(bufCtl.blkPtrs[0], idx + 1000, buf);
        freeBlockInBuffer(bufCtl.blkPtrs[0], buf);
    }
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
    // 归并
    int idx = 0;
    for (int group = 0; group < bufCtl.nSet; group++) {
        // 最后一组不足 commonSize个块
#if DEBUG == TPMMS_VERSION
        int cnt = -1;
        printf(">>>>> GROUP: %d\n", group + 1);
#endif
        int tmp = getSize(&bufCtl, group);
        for (int i = 0; i < tmp; i++) {
            bufCtl.blkPtrs[i] = readBlockFromDisk(1000 + startBlock + i + group * bufCtl.commonSize, buf);
            if (bufCtl.blkPtrs[i] == NULL) {
                printf("Reading Block Failed! Group: %d, block: %d\n", group, i);
                return -1;
            }
        }
        bufCtl.blkPtrs[7] = getNewBlockInBuffer(buf);

        tuple ret;
        ret.finish = 0;
        bufCtl.curGroup = group;
        while (!ret.finish) {
            ret = getMin(&bufCtl);
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
#if DEBUG == TPMMS_VERSION
                printBlk(bufCtl.blkPtrs[7]);
                printf("\n");
#endif
                writeNext(bufCtl.blkPtrs[7], idx + startBlock + 2000 + 1);
                writeBlockToDisk(bufCtl.blkPtrs[7], idx + startBlock + 2000, buf);
                bufCtl.blkCnt[7] = 0;
                idx++;
                freeBlockInBuffer(bufCtl.blkPtrs[7], buf);
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
    // -----------------------------------
    for (int group = 1; group < bufCtl.nSet; group++) {
        bufCtl.blkPtrs[group] = readBlockFromDisk(2000 + startBlock + group * bufCtl.commonSize - 1, buf);
        writeNext(bufCtl.blkPtrs[group], 0);
        writeBlockToDisk(bufCtl.blkPtrs[group], 2000 + startBlock + group * bufCtl.commonSize - 1, buf);
        freeBlockInBuffer(bufCtl.blkPtrs[group], buf);
        bufCtl.blkPtrs[group] = NULL;
    }
    bufCtl.blkPtrs[0] = readBlockFromDisk(2000 + endBlock, buf);
    writeNext(bufCtl.blkPtrs[0], 0);
    writeBlockToDisk(bufCtl.blkPtrs[0], 2000 + endBlock, buf);
    freeBlockInBuffer(bufCtl.blkPtrs[0], buf);
    bufCtl.blkPtrs[0] = NULL;


    // -------------------------------
    int k = 7;
    int num = bufCtl.totalBlocks % k == 0 ? bufCtl.totalBlocks / k : bufCtl.totalBlocks / k + 1;
    bufCtl.curGroup = num;
    idx = 0;
    for (int i = 0; i < num; i++) {
        bufCtl.blkPtrs[i] = readBlockFromDisk(2000 + startBlock + i * k, buf);
        if (bufCtl.blkPtrs[i] == NULL) {
            printf("Reading Block Failed! Group: %d\n", i);
            return -1;
        }
    }
    bufCtl.blkPtrs[7] = getNewBlockInBuffer(buf);

    tuple ret;
    while (!ret.finish) {
        ret = getMinV2(&bufCtl, buf);
        if (ret.finish) {
            for (int i = 0; i < num; i++) {
                freeBlockInBuffer(bufCtl.blkPtrs[i], buf);
                bufCtl.blkCnt[i] = 0;
            }
            break;
        }
        writeAttribute(bufCtl.blkPtrs[7], bufCtl.blkCnt[7], ret.X, ret.Y);
        bufCtl.blkCnt[7]++;
        if (bufCtl.blkCnt[7] == 7) {
            writeBlockToDisk(bufCtl.blkPtrs[7], idx + startBlock + 3000, buf);
            bufCtl.blkCnt[7] = 0;
            idx++;
            freeBlockInBuffer(bufCtl.blkPtrs[7], buf);
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

}