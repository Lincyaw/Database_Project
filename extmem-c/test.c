#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linearSearch.h"
#include "2stageSort.h"

int main(int argc, char **argv) {
    Buffer buf; /* A buffer */
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
//    linearSelect(&buf, 30, -1, 1, 16);
//    TPMMS(&buf, 1, 16);
//    TPMMS(&buf, 17, 48);
//    generateIndex(&buf, 3001, 3016);
//    generateIndex(&buf, 3017, 3048);
//    idxSearch(&buf, 42, -1, 3017, 3048);



    unsigned char *blk;
//    for(int i = 3001; i<=3016; i++){
//        blk= readBlockFromDisk(i,&buf);
//        printBlk(blk);
//        freeBlockInBuffer(blk,&buf);
//    }
//    printf("-------------------------------------\n");
//
    for(int i = 3017; i<=3048; i++){
        blk= readBlockFromDisk(i,&buf);
        printBlk(blk);
        freeBlockInBuffer(blk,&buf);
    }


//    sortMergeJoin(&buf,1,16,17,48);
    intersection(&buf,1,16,17,48);
    printf("%lu\n", buf.numIO);
    return 0;
}