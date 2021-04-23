//
// Created by llincyaw on 2021/4/22.
//

#ifndef EXTMEM_C_LINEARSEARCH_H
#define EXTMEM_C_LINEARSEARCH_H
#include "extmem.h"

void select(Buffer *buf, int valueC, int valueD, int startBlock, int endBlock);
int sortRelation(Buffer *buf, int startBlock, int endBlock);
#endif //EXTMEM_C_LINEARSEARCH_H
