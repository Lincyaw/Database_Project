//
// Created by llincyaw on 2021/4/25.
//

#ifndef EXTMEM_C_2STAGESORT_H
#define EXTMEM_C_2STAGESORT_H
#include "extmem.h"
int TPMMS(Buffer *buf, int startBlock, int endBlock);
int sortMergeJoin(Buffer *buf, int RStart, int REnd, int SStart,int SEnd);
int intersection(Buffer *buf, int RStart, int REnd, int SStart, int SEnd);
#endif //EXTMEM_C_2STAGESORT_H
