#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linearSearch.h"

int main(int argc, char **argv) {
    Buffer buf; /* A buffer */
    unsigned char *blk; /* A pointer to a block */
    int i = 0;

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    selectS(&buf, 50, -1);
//    /* Get a new block in the buffer */
//    blk = getNewBlockInBuffer(&buf);
//
//    /* Fill data into the block */
//    for (i = 0; i < 8; i++)
//        *(blk + i) = 'a' + i;
//
//    /* Write the block to the hard disk */
//    if (writeBlockToDisk(blk, 8888, &buf) != 0)
//    {
//        perror("Writing Block Failed!\n");
//        return -1;
//    }
//
//    /* Read the block from the hard disk */
//    if ((blk = readBlockFromDisk(1, &buf)) == NULL)
//    {
//        perror("Reading Block Failed!\n");
//        return -1;
//    }
//
//    /* Process the data in the block */
//    int X = -1;
//    int Y = -1;
//    int addr = -1;
//
//    char str[5];
//    printf("block 1:\n");
//    for (i = 0; i < 7; i++) //一个blk存7个元组加一个地址
//    {
//        getAttribute(blk,i,&X,&Y);
//        printf("(%d, %d) ", X, Y);
//    }
//    addr = getNext(blk,i);
//    printf("\nnext address = %d \n", addr);
//
//
//    printf("\n");
//    printf("IO's is %d\n", buf.numIO); /* Check the number of IO's */

    return 0;
}

/*
 * block 1:
(45, 1328) (49, 1526) (59, 1702) (43, 1899) (44, 1615) (45, 1203) (34, 1525)
next address = 2

IO's is 2

 * */