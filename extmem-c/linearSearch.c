//
// Created by llincyaw on 2021/4/22.
//

#include "linearSearch.h"

void selectS(Buffer *buf, int valueC, int valueD) {
    unsigned char *blk = NULL;
    unsigned char *target = getNewBlockInBuffer(buf);

    for (int idx = RELATION_S_BEGIN; idx <= RELATION_S_END; idx++) {
        // 读一个块
        if ((blk = readBlockFromDisk(idx, buf)) == NULL) {
            perror("Reading Block Failed!\n");
            return;
        }
        //一个blk存7个元组
        for (int i = 0; i < 7; i++) {
            int X = -1, Y = -1;
            getAttribute(blk, i, &X, &Y);
            if (valueC == X) {
                // todo: 维护一个指针, target block, write the tuple to the block
                // if block is full, write it to the disk
            }
        }

        freeBlockInBuffer(blk, buf);
    }
    if (valueC != -1 && valueD != -1) {

    } else if (valueC != -1) {

    } else if (valueD != -1) {

    } else {

    }
}