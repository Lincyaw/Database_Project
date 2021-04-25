#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linearSearch.h"
#include "2stageSort.h"
int main(int argc, char **argv) {
    Buffer buf; /* A buffer */
    unsigned char *blk; /* A pointer to a block */

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
//    TPMMS(&buf, 1, 16);
//    TPMMS(&buf, 17, 48);
    generateIndex(&buf,3017,3048);
//    selectTable(&buf,50, -1, 17,48);
    printf("%lu\n", buf.numIO);
    return 0;
}