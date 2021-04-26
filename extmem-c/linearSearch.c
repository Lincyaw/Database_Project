//
// Created by llincyaw on 2021/4/22.
//

#include "linearSearch.h"


void selectTable(Buffer *buf, int valueC, int valueD, int startBlock, int endBlock) {
    unsigned char *blk = NULL;
    int outputIndex = 300;
    search(buf,valueC,valueD,startBlock,endBlock, outputIndex);
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
                    printf("%d,%d,%d\n", X, Y, i);
                    writeAttribute(index, tuple++, X, i);
                    flag = 0;
                    if (tuple == 7 || i == endBlock - 1) {
                        writeBlockToDisk(index, 4000 + blockIndex + startBlock, buf);
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
    writeBlockToDisk(index, 4000 + blockIndex + startBlock, buf);
    freeBlockInBuffer(index, buf);
}

void idxSearch(Buffer *buf, int valueC, int valueD, int startBlock, int endBlock) {
    unsigned char *blk = NULL;
    int start = -1, end = -1;
    int index;
    if (startBlock >= 3001 && startBlock <= 3016 && endBlock >= 3001 && endBlock <= 3016) {
        index = 7001;
    } else if (startBlock >= 3017 && startBlock <= 3048 && endBlock >= 3017 && endBlock <= 3048) {
        index = 7017;
    } else {
        return;
    }
    blk = readBlockFromDisk(index++, buf);
    while (start == -1 || end == -1) {
        for (int i = 0; i < 7; i++) {
            int X, Y;
            getAttribute(blk, i, &X, &Y);
            if (X <= valueC) {
                start = Y;
            }
            if (X >= valueC) {
                end = Y;
            }
            if (start != -1 && end != -1) {
                break;
            }
        }
        freeBlockInBuffer(blk, buf);
        if (start != -1 && end != -1) {
            break;
        }
        blk = readBlockFromDisk(index++, buf);
    }

    end = end > start ? end : start;
    search(buf,valueC,valueD,start,end,5000);
}