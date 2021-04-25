#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linearSearch.h"

int main(int argc, char **argv) {
    Buffer buf; /* A buffer */
    unsigned char *blk; /* A pointer to a block */

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
//    sortRelation(&buf, 17, 48);
    selectTable(&buf,50, -1, 17,48);
//    for (int i = 17; i <= 48; i++) {
//        blk = readBlockFromDisk(i, &buf);
//        printBlk(blk);
//        freeBlockInBuffer(blk, &buf);
//    }
    printf("%lu\n", buf.numIO);
    return 0;
}