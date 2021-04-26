#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extmem.h"


#define A_MAX 40
#define A_MIN 1
#define B_MAX 1000
#define B_MIN 1
#define C_MAX 60
#define C_MIN 20
#define D_MAX 1000
#define D_MIN 1
#define VALUE_MAX 99999
#define VALUE_SIZE 4    //属性值所占字节
#define NONE "\033[m"
#define YELLOW "\033[40;33m"


const unsigned int tuples_number_per_block = 7;
const unsigned int s_begin_addr = 17;
const unsigned int s_end_addr = 48;
const unsigned int s_sort_addr1 = 417;
const unsigned int s_sort_addr = 317;
const unsigned int s_sort_end_addr = 348;
const unsigned int s_blocks_num = 32;

const unsigned int r_begin_addr = 1;
const unsigned int r_end_addr = 16;
const unsigned int r_sort_addr1 = 401;
const unsigned int r_sort_addr = 301;
const unsigned int r_sort_end_addr = 316;
const unsigned int r_blocks_num = 16;

const unsigned int linear_search_ans_addr = 200;
const unsigned int sort1_bias = 400;
const unsigned int r_index_addr = 500;
const unsigned int s_index_addr = 550;
const unsigned int index_search_ans_addr = 600;
const unsigned int projection_ans_addr = 800;
const unsigned int sort_merge_join_ans_addr = 1000;
const unsigned int intersection_ans_addr = 1200;
const unsigned int bufSize = 520;
const unsigned int blkSize = 64;


typedef struct Tuples
{
    int x;
    int y;
} Tuple;

// 自定义工具函数
// 从buffer中指定的块中，取出指定块中指定位置的元组的指定属性
int getTupleValue(unsigned char *blk, int offset, int attr_index);
// 从buffer中指取出指定位置的元组的指定属性
int getTupleValueFromBuffer(Buffer *buffer, int offset, int attr_index);
// 将给定的值赋值给buffer指定块中指定位置的元组的指定属性
void pushTupleValue(unsigned char *blk, int offset, int attr_index, int value);
// 将给定的值赋值给buffer指定块中指定位置的元组的指定属性
void pushTupleValueToBuffer(Buffer *buffer, int offset, int attr_index, int value);
// 清空buffer中指定块的值，将块内的值全部设置为' '
void refreshBlockInBuffer(Buffer *buffer, unsigned char *blk);
// 对buffer内的元组进行冒泡排序
void bufferBubbleSort(Buffer *buffer);
// 在TPMMS的归并排序中，获取最小数值元组所在的子集合
int getMinTupleIndex(unsigned char *compare_block, int subset_num);
// 输出TPMMS结果
void outputSortedRelation(Buffer *buffer, unsigned int relation_addr);
// 建立并初始化索引
int initIndex(Buffer *buffer, char relation);

// 关系操作函数
int linearSearch(Buffer *buffer, unsigned int relation_addr, int attr_index, int search_value);
int TPMMS(Buffer *buffer, unsigned int relation_addr);
int indexSearch(Buffer *buffer, unsigned int relation_addr, int attr_index, int search_value);
int projection(Buffer *buffer, unsigned int start_addr);
int sortMergeJoin(Buffer *buffer);
int intersection(Buffer *buffer);



int main()
{
    Buffer buffer;
    int linear_search_result;
    printf("sac");
    system("cls"); //清屏，使输出有颜色
    if (!initBuffer(bufSize, blkSize, &buffer)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }


    //1.线性搜索
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于线性搜索的选择算法 R.A=30:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    linear_search_result = linearSearch(&buffer, r_begin_addr, 1, 30);
    if(linear_search_result != 1){
        if(linear_search_result == 0){
            printf("无结果\n");
        }
        else{
            printf("线性搜索失败\n");
        }
    }
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于线性搜索的选择算法 S.C=23:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    linear_search_result = linearSearch(&buffer, s_begin_addr, 1, 23);
    if(linear_search_result != 1){
        if(linear_search_result == 0){
            printf("无结果\n");
        }
        else{
            printf("线性搜索失败\n");
        }
    }


    // 2.TPMMS
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"两阶段多路归并排序算法:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    if(TPMMS(&buffer, r_begin_addr)){
        printf(YELLOW"关系R排序完成，输出到磁盘的起始地址为%d\n"NONE, r_sort_addr);
    }
    outputSortedRelation(&buffer, r_sort_addr);
    if(TPMMS(&buffer, s_begin_addr)){
        printf(YELLOW"关系S排序完成，输出到磁盘的起始地址为%d\n"NONE, s_sort_addr);
    }
    outputSortedRelation(&buffer, s_sort_addr);


    //3.基于索引的关系选择算法
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于索引的关系选择算法R.A=30:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    if(initIndex(&buffer, 'R')){
        if (!initBuffer(bufSize, blkSize, &buffer)) {
            perror("Buffer Initialization Failed!\n");
            return -1;
        }
        indexSearch(&buffer, r_sort_addr, 1, 30);
    }
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于索引的关系选择算法S.C=23:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    if(initIndex(&buffer, 'S')){
        if (!initBuffer(bufSize, blkSize, &buffer)) {
            perror("Buffer Initialization Failed!\n");
            return -1;
        }
        indexSearch(&buffer, s_sort_addr, 1, 23);
    }

    //4.投影
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于排序的投影算法并去重:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    if(!projection(&buffer, r_sort_addr)){
        printf(YELLOW"投影并去重失败\n"NONE);
    }


    //5.基于排序的连接操作算法
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于排序的连接操作算法:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    if(sortMergeJoin(&buffer)){
        printf(YELLOW"无结果\n"NONE);
    }

    //6.基于排序的集合交运算
    printf(YELLOW"\n==============================\n"NONE);
    printf(YELLOW"基于排序的集合交运算:\n"NONE);
    printf(YELLOW"==============================\n"NONE);
    if(intersection(&buffer)){
        printf(YELLOW"无交集\n"NONE);
    }


    return 0;
}

int getTupleValue(unsigned char *blk, int offset, int attr_index)
{
    int i;
    char res[5];
    for (i = 0; i < VALUE_SIZE; i++){
        res[i] = *(blk + offset*VALUE_SIZE*2 + (attr_index-1)*VALUE_SIZE + i);
    }
    return atoi(res);
}

int getTupleValueFromBuffer(Buffer *buffer, int offset, int attr_index)
{
    int i;
    char res[5];
    for (i = 0; i < VALUE_SIZE; i++){
        res[i] = *(buffer->data+ offset/8 + 1 + offset*VALUE_SIZE*2 + (attr_index-1)*VALUE_SIZE + i);
    }
    return atoi(res);
}

void pushTupleValue(unsigned char *blk, int offset, int attr_index, int value)
{
    int i;
    char str[5] = "    ";
    itoa(value, str, 10);
    for(i = 0; i < VALUE_SIZE; i++){
        *(blk + offset*VALUE_SIZE*2 + (attr_index-1)*VALUE_SIZE + i) = str[i];
    }
}

void pushTupleValueToBuffer(Buffer *buffer, int offset, int attr_index, int value)
{
    int i;
    char str[5] = "    ";
    itoa(value, str, 10);
    for(i = 0; i < VALUE_SIZE; i++){
        *(buffer->data+ offset/8 + 1 + offset*VALUE_SIZE*2 + (attr_index-1)*VALUE_SIZE + i) = str[i];
    }
}

void refreshBlockInBuffer(Buffer *buffer, unsigned char *blk)
{
    int i;
    char zero[5] = "    ";
    for(i=0;i<buffer->blkSize;i+=VALUE_SIZE){
        memcpy(blk+i, zero, VALUE_SIZE);
    }
}

int linearSearch(Buffer *buffer, unsigned int relation_addr, int attr_index, int search_value)
{
    // 判断所选的列是否在关系列索引中
    if(attr_index != 1 && attr_index != 2){
        perror("There are only 2 attributes and attribute index should be 1 or 2!\n");
        return -2;
    }
    // 判断查询的值是否超过列的域
    unsigned int end_addr;
    if (relation_addr == r_begin_addr) {
        end_addr = r_end_addr;
        if ((attr_index == 1 && (search_value > A_MAX || search_value < A_MIN)) ||
            (attr_index == 2 && (search_value > B_MAX || search_value < B_MIN))) {
            return 1;
        }
    } else {
        end_addr = s_end_addr;
        if ((attr_index == 1 && (search_value > C_MAX || search_value < C_MIN)) ||
            (attr_index == 2 && (search_value > D_MAX || search_value < D_MIN))) {
            return 1;
        }
    }

    unsigned char *read_block;
    if((read_block = readBlockFromDisk(relation_addr, buffer)) == NULL){
        perror("Reading Block Failed!\n");
        return -1;
    }

    unsigned int is_found = 0;
    unsigned char *write_block = getNewBlockInBuffer(buffer);
    refreshBlockInBuffer(buffer, write_block);
    unsigned int next_addr = linear_search_ans_addr;
    unsigned int write_block_used = 0;
    unsigned int next_read_addr = relation_addr;
    unsigned int res_count = 0;

    while (next_read_addr <= end_addr){
        int i;
        // 读取块元组
        printf("读取数据块%u\n", next_read_addr);
        for(i = 0; i < tuples_number_per_block; i++){
            int x, y;
            x = getTupleValue(read_block, i, 1);
            y = getTupleValue(read_block, i, 2);
            // 找到结果就写入缓冲区
            if ((x == search_value && attr_index == 1) || (y == search_value && attr_index == 2)) {
                is_found = 1;
                printf("( X=%d , Y=%d )\n",x, y);
                res_count += 1;
                pushTupleValue(write_block, write_block_used, 1, x);
                pushTupleValue(write_block, write_block_used, 2, y);
                write_block_used += 1;
                // 填完了第七个元组后还需填写后继磁盘块地址,并写入磁盘
                if(write_block_used == tuples_number_per_block){
                    next_addr += 1;
                    pushTupleValue(write_block, write_block_used, 1, next_addr);
                    if (writeBlockToDisk(write_block, next_addr, buffer) != 0) {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    write_block = getNewBlockInBuffer(buffer);
                    refreshBlockInBuffer(buffer, write_block);
                    write_block_used = 0;
                }
            }
        }
        if(next_read_addr == end_addr){
            break;
        }
        next_read_addr += 1;
        freeBlockInBuffer(read_block, buffer);
        if ((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL) {
            perror("Reading Block Failed!\n");
            return -1;
        }
    }

    if(write_block_used == 0){
        // 多申请了一个块的输出缓冲 需要将多申请的块释放
        freeBlockInBuffer(write_block, buffer);
    }
    else{
        pushTupleValue(write_block, tuples_number_per_block, 1, next_addr+1);
        if (writeBlockToDisk(write_block, next_addr, buffer) != 0){
            perror("Writing Block Failed!\n");
            return -1;
        }
        freeBlockInBuffer(write_block, buffer);
    }

    printf("注：结果从磁盘的第%u块开始写入\n\n", linear_search_ans_addr);
    printf(YELLOW"满足条件的元组一共有%u个\n\n"NONE, res_count);
    printf(YELLOW"IO读写一共%d次\n"NONE, buffer->numIO);

    return is_found;

}

void bufferBubbleSort(Buffer *buffer){
    int i ,j;
    unsigned int x, x1, y, y1;
    // 计算buffer可以存储的元组数量n
    int n = buffer->numAllBlk * (tuples_number_per_block + 1);
    for(i = 0; i < n; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            // 先获取属性值
            x = getTupleValueFromBuffer(buffer, j, 1);
            x1 = getTupleValueFromBuffer(buffer, j + 1, 1);
            y = getTupleValueFromBuffer(buffer, j, 2);
            y1 = getTupleValueFromBuffer(buffer, j + 1, 2);
            if(y1 == 0 && j+2<n){
                // 说明偏移量为j+1对应的元组存储的是磁盘后继地址
                // 则应将偏移量j和j+2的元组属性值进行比较
                x1 = getTupleValueFromBuffer(buffer, j + 2, 1);
                y1 = getTupleValueFromBuffer(buffer, j + 2, 2);
                if (x > x1) {
                    pushTupleValueToBuffer(buffer, j + 2, 1, x);
                    pushTupleValueToBuffer(buffer, j + 2, 2, y);
                    pushTupleValueToBuffer(buffer, j, 1, x1);
                    pushTupleValueToBuffer(buffer, j, 2, y1);
                }
            } else if(y1 == 0 && j+2>=n){
                // 说明偏移量为j+1对应的元组存储的是磁盘后继地址
                ;
            } else if(y == 0){
                // 当前元组存储的是地址，无需进行比较操作
                ;
            } else{
                // 则应将偏移量j和j+1的元组属性值进行比较
                if (x > x1) {
                    //偏移量j的元组指定属性值大于偏移量j+1的元组指定属性值，进行交换操作
                    y = getTupleValueFromBuffer(buffer, j, 2);
                    y1 = getTupleValueFromBuffer(buffer, j + 1, 2);
                    pushTupleValueToBuffer(buffer, j + 1, 1, x);
                    pushTupleValueToBuffer(buffer, j + 1, 2, y);
                    pushTupleValueToBuffer(buffer, j, 1, x1);
                    pushTupleValueToBuffer(buffer, j, 2, y1);
                }
            }
        }
    }
    // 更新地址信息
    for(i = 0; i < buffer->numAllBlk; i++){
        x = getTupleValueFromBuffer(buffer, (i+1) * 8 - 1, 1);
        pushTupleValueToBuffer(buffer, (i+1) * 8 - 1, 1, x + sort1_bias);
    }
}

int getMinTupleIndex(unsigned char *compare_block, int subset_num)
{
    int i, res=-1, x;
    unsigned int min = VALUE_MAX;
    for(i = 0; i< subset_num ; i++){
        x = getTupleValue(compare_block, i, 1);
        if(x < min && x !=0){
            min = x;
            res = i;
        }
    }
    return res;
}

int TPMMS(Buffer *buffer, unsigned int relation_addr)
{
    // 设置TPMMS结果地址，计算划分的子集合数
    // R：2个子集合，每个子集合8块
    // S：4个子集合，每个子集合8块
    unsigned int next_write_addr, res1_addr, res_addr;
    unsigned int subset_num;
    if(relation_addr == r_begin_addr){
        res1_addr = r_sort_addr1;
        res_addr = r_sort_addr;
        next_write_addr = r_sort_addr1;
        subset_num = r_blocks_num / (tuples_number_per_block + 1);
    }else{
        res1_addr = s_sort_addr1;
        res_addr = s_sort_addr;
        next_write_addr = s_sort_addr1;
        subset_num = s_blocks_num / (tuples_number_per_block + 1);
    }


    //===========================================================================
    // 第一阶段:将子集合装入buffer并排序
    unsigned char *read_block;
    unsigned int next_read_addr = relation_addr-1;
    if (!initBuffer(bufSize, blkSize, buffer)) {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    for (int i =0; i < subset_num; i++) {
        for (int j = 0; j < buffer->numAllBlk; j++){
            next_read_addr += 1;
            if ((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL) {
                perror("Reading Block Failed!\n");
                return -1;
            }
        }
        bufferBubbleSort(buffer);
        for (int j = 0; j < buffer->numAllBlk; j++){
            if((writeBlockToDisk(buffer->data + j*blkSize + j + 1, next_write_addr, buffer)) != 0){
                perror("Writing Block Failed!\n");
                return -1;
            }
            next_write_addr += 1;
        }
        if (!initBuffer(bufSize, blkSize, buffer)) {
            perror("Buffer Initialization Failed!\n");
            return -1;
        }
    }

    //===========================================================================
    // 第二阶段：子集合归并排序
    int i;
    next_write_addr = res_addr;
    unsigned char *write_block = getNewBlockInBuffer(buffer);
    refreshBlockInBuffer(buffer, write_block);
    unsigned int write_block_used = 0;
    // 用于存储子集合块在buffer中的地址
    unsigned char **subset_block = (unsigned char **)malloc(subset_num * sizeof(unsigned char*));
    for(i = 0; i < subset_num; i++){
        subset_block[i] = (unsigned char *)malloc(sizeof(unsigned char*));
    }
    unsigned char *compare_block = getNewBlockInBuffer(buffer); // 比较块
    int *subset_block_index = (int *)calloc(subset_num, sizeof(int)); // 用于获取子集合一块中的下一个元素
    int *subset_block_sorted = (int *)calloc(subset_num, sizeof(int)); // 用于标记子集合中已排序的块，看是否还有下一块

    // 依次读取子集合中的一块
    for(i = 0; i < subset_num; i++){
        if ((subset_block[i] = readBlockFromDisk(res1_addr + i * buffer->numAllBlk, buffer)) == NULL)
        {
            perror("Reading Block Failed!\n");
            return -1;
        }
        pushTupleValue(compare_block, i, 1, getTupleValue(subset_block[i], 0, 1));
        pushTupleValue(compare_block, i, 2, getTupleValue(subset_block[i], 0, 2));
    }


    while(1){
        int min = getMinTupleIndex(compare_block, subset_num);
        // 所有元组都归并完了，结束算法
        if(min == -1){
            break;
        }
        pushTupleValue(write_block, write_block_used, 1, getTupleValue(compare_block, min, 1));
        pushTupleValue(write_block, write_block_used, 2, getTupleValue(compare_block, min, 2));
        subset_block_index[min] += 1;
        write_block_used += 1;

        // 待写块满，写入磁盘
        if(write_block_used == tuples_number_per_block){
            next_write_addr += 1;
            pushTupleValue(write_block, write_block_used, 1, next_write_addr);
            if (writeBlockToDisk(write_block, next_write_addr-1, buffer) != 0){
                perror("Writing Block Failed!\n");
                return -1;
            }
            write_block = getNewBlockInBuffer(buffer);
            write_block_used = 0;
            refreshBlockInBuffer(buffer, write_block);
        }

        //若没有下一个元素则读入下一块
        if(subset_block_index[min] == 7){
            next_read_addr = getTupleValue(subset_block[min], tuples_number_per_block, 1);
            dropBlockOnDisk(next_read_addr-1);
            freeBlockInBuffer(subset_block[min], buffer);
            //若没有下一块,让compare_block该子集合处为0
            subset_block_index[min] = 0;
            subset_block_sorted[min] += 1;
            if(subset_block_sorted[min] != buffer->numAllBlk){
                if((subset_block[min] = readBlockFromDisk(next_read_addr, buffer)) == NULL){
                    perror("Reading Block Failed!\n");
                    return -1;
                }
            }
            else {
                pushTupleValue(compare_block, min, 1, 0);
                pushTupleValue(compare_block, min, 2, 0);
            }
        }
        //获取下一个元素
        if(subset_block_sorted[min] != buffer->numAllBlk){
            pushTupleValue(compare_block, min, 1, getTupleValue(subset_block[min], subset_block_index[min], 1));
            pushTupleValue(compare_block, min, 2, getTupleValue(subset_block[min], subset_block_index[min], 2));
        }
    }

    //释放buffer
    for(i = 0; i < subset_num; i++){
        freeBlockInBuffer(subset_block[i], buffer);
    }
    freeBlockInBuffer(compare_block, buffer);
    freeBlockInBuffer(write_block, buffer);
//    for(i = 0; i < subset_num; i++){
//        free(subset_block[i]);
//    }
//    free(subset_block);
    return 1;
}

int initIndex(Buffer *buffer, char relation)
{
    unsigned int next_write_addr, next_read_addr, end_addr;
    if (relation == 'R') {
        next_read_addr = r_sort_addr;
        end_addr = r_sort_end_addr;
        next_write_addr = r_index_addr;
    } else {
        next_read_addr = s_sort_addr;
        end_addr = s_sort_end_addr;
        next_write_addr = s_index_addr;
    }

    unsigned char *read_block;
    unsigned int write_block_used = 0;
    unsigned char *write_block = getNewBlockInBuffer(buffer);
    refreshBlockInBuffer(buffer, write_block);
    while(next_read_addr<=end_addr){
        if((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL){
            perror("Reading Block Failed!\n");
            return -1;
        }
        if(write_block_used == tuples_number_per_block){
            next_write_addr += 1;
            pushTupleValue(write_block, write_block_used, 1, next_write_addr);
            if (writeBlockToDisk(write_block, next_write_addr-1, buffer) != 0) {
                perror("Writing Block Failed!\n");
                return -1;
            }
            write_block = getNewBlockInBuffer(buffer);
            refreshBlockInBuffer(buffer, write_block);
            write_block_used = 0;
        }
        // 将每一块第一个元组的索引键值和块地址作为索引项
        pushTupleValue(write_block, write_block_used, 1, getTupleValue(read_block, 0, 1));
        pushTupleValue(write_block, write_block_used, 2, getTupleValue(read_block, tuples_number_per_block, 1)-1);
        write_block_used += 1;
        freeBlockInBuffer(read_block, buffer);
        next_read_addr += 1;
    }
    if(write_block_used == 0){
        // 多申请了一个块的输出缓冲 需要将多申请的块释放
        // 并将上一块后继地址写为0
        if((read_block = readBlockFromDisk(next_write_addr-1, buffer)) == NULL){
            perror("Reading Block Failed!\n");
            return -1;
        }
        pushTupleValue(read_block, tuples_number_per_block, 1, 0);
        if (writeBlockToDisk(read_block, next_write_addr-1, buffer) != 0) {
            perror("Writing Block Failed!\n");
            return -1;
        }
        freeBlockInBuffer(write_block, buffer);
    }
    else{
        // 后继地址写为0表示结束
        pushTupleValue(write_block, tuples_number_per_block, 1, 0);
        if (writeBlockToDisk(write_block, next_write_addr, buffer) != 0){
            perror("Writing Block Failed!\n");
            return -1;
        }
        freeBlockInBuffer(write_block, buffer);
    }
    return 1;
}

int indexSearch(Buffer *buffer, unsigned int relation_addr, int attr_index, int search_value)
{
    // 判断所选的列是否在关系列索引中
    if(attr_index != 1 && attr_index != 2){
        perror("There are only 2 attributes and attribute index should be 1 or 2!\n");
        return -2;
    }
    // 判断查询的值是否超过列的域
    unsigned int end_addr;
    unsigned int next_read_addr;
    if (relation_addr == r_sort_addr) {
        next_read_addr = r_index_addr;
        end_addr = r_sort_end_addr;
        if ((attr_index == 1 && (search_value > A_MAX || search_value < A_MIN)) ||
            (attr_index == 2 && (search_value > B_MAX || search_value < B_MIN))) {
            return 1;
        }
    } else {
        end_addr = s_sort_end_addr;
        next_read_addr = s_index_addr;
        if ((attr_index == 1 && (search_value > C_MAX || search_value < C_MIN)) ||
            (attr_index == 2 && (search_value > D_MAX || search_value < D_MIN))) {
            return 1;
        }
    }

    unsigned char *read_block;
    if((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL){
        perror("Reading Block Failed!\n");
        return -1;
    }
    printf("读入索引块%d\n",next_read_addr);

    unsigned int is_found = 0, read_value, search_addr;
    int read_block_count = 0;

    unsigned int res_count = 0;
    while(1){
        read_value = getTupleValue(read_block, read_block_count, 1);
        search_addr = getTupleValue(read_block, read_block_count, 2);
        if(read_value >= search_value){
            is_found = 1;
            break;
        }
        read_block_count += 1;
        if(read_block_count==tuples_number_per_block){
            printf("没有满足条件的元组\n");
            next_read_addr = getTupleValue(read_block, tuples_number_per_block, 1);
            freeBlockInBuffer(read_block, buffer);
            if((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL){
                perror("Reading Block Failed!\n");
                return -1;
            }
            printf("读入索引块%d\n",next_read_addr);
            read_block_count = 0;
        }
        if(next_read_addr == 0){
            break;
        }
    }

    // 根据索引项的地址寻找元组
    if(next_read_addr == 0){
        return 0;
    }
    unsigned char *write_block = getNewBlockInBuffer(buffer);
    refreshBlockInBuffer(buffer, write_block);
    unsigned int next_write_addr = index_search_ans_addr;
    unsigned int write_block_used = 0;

    next_read_addr = search_addr-1;
    read_block_count = 0;
    freeBlockInBuffer(read_block, buffer);
    if((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL){
        perror("Reading Block Failed!\n");
        return -1;
    }
    printf("读取数据块%d\n", next_read_addr);
    while(1){
        unsigned int a = getTupleValue(read_block, read_block_count, 1);
        unsigned int b = getTupleValue(read_block, read_block_count, 2);
        read_block_count += 1;
        if(a < search_value){
            read_block_count += 1;
        } else if (a == search_value){
            is_found = 1;
            res_count += 1;
            printf("( X=%u , Y=%u )\n",a, b);
            pushTupleValue(write_block, write_block_used, 1, a);
            pushTupleValue(write_block, write_block_used, 2, b);
            write_block_used += 1;
        } else {
            break;
        }
        if(read_block_count == tuples_number_per_block){
            next_read_addr = getTupleValue(read_block, tuples_number_per_block, 1);
            freeBlockInBuffer(read_block, buffer);
            if((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL){
                perror("Reading Block Failed!\n");
                return -1;
            }
            printf("读取数据块%d\n", next_read_addr);
            read_block_count = 0;
        }
        if(write_block_used == tuples_number_per_block){
            next_write_addr += 1;
            pushTupleValue(write_block, write_block_used, 1, next_write_addr);
            if (writeBlockToDisk(write_block, next_write_addr-1, buffer) != 0) {
                perror("Writing Block Failed!\n");
                return -1;
            }
            write_block = getNewBlockInBuffer(buffer);
            refreshBlockInBuffer(buffer, write_block);
            write_block_used = 0;
        }
    }
    if(write_block_used == 0){
        freeBlockInBuffer(read_block, buffer);
        freeBlockInBuffer(write_block, buffer);
    }
    else{
        pushTupleValue(write_block, tuples_number_per_block, 1, next_write_addr+1);
        if (writeBlockToDisk(write_block, next_write_addr, buffer) != 0){
            perror("Writing Block Failed!\n");
            return -1;
        }
        freeBlockInBuffer(write_block, buffer);
    }

    printf("注：结果从磁盘的第%u块开始写入\n\n", index_search_ans_addr);
    printf(YELLOW"满足条件的元组一共有%u个\n\n"NONE, res_count);
    printf(YELLOW"IO读写一共%d次\n"NONE, buffer->numIO);

    return is_found;
}

int projection(Buffer *buffer, unsigned int relation_addr)
{
    unsigned char *read_block;
    if ((read_block = readBlockFromDisk(relation_addr, buffer)) == NULL) {
        perror("Reading Block Failed!\n");
        return -1;
    }
    printf("读入数据块%d\n", relation_addr);
    unsigned int end_addr = r_sort_addr + r_blocks_num - 1;
    unsigned char *write_block = getNewBlockInBuffer(buffer);
    unsigned int next_write_addr = projection_ans_addr;
    unsigned int next_read_addr;
    int write_block_used = 1;

    unsigned  int pre_value, cur_value, res_count = 1;
    pre_value = getTupleValue(read_block, 0, 1);
    pushTupleValue(write_block, write_block_used,1, getTupleValue(read_block, 0, 1));
    pushTupleValue(write_block, write_block_used,2, getTupleValue(read_block, 0, 2));
    printf("(X=%u)\n", pre_value);

    while (1){
        int i;
        for(i = 0; i < tuples_number_per_block; i++){
            cur_value = getTupleValue(read_block, i, 1);
            if(pre_value != cur_value){
                printf("(X=%u)\n", cur_value);
                pushTupleValue(write_block, write_block_used,1, cur_value);
//                pushTupleValue(write_block, write_block_used,2, getTupleValue(read_block, i, 2));
                write_block_used += 1;
                pre_value = cur_value;
                res_count += 1;
            }
            // 存后继地址
            if(write_block_used == tuples_number_per_block){
                pushTupleValue(write_block, write_block_used, 1, next_write_addr+1);
                if (writeBlockToDisk(write_block, next_write_addr, buffer) != 0){
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                next_write_addr += 1;
                write_block = getNewBlockInBuffer(buffer);
                refreshBlockInBuffer(buffer, write_block);
                write_block_used = 0;
            }
        }

        next_read_addr = getTupleValue(read_block, tuples_number_per_block, 1);
        // 读取完关系所有的块后结束读取循环
        if(next_read_addr > end_addr){
            freeBlockInBuffer(read_block, buffer);
            break;
        }
        // 否则读取下一块
        freeBlockInBuffer(read_block, buffer);
        if((read_block = readBlockFromDisk(next_read_addr, buffer)) == NULL){
            perror("Reading Block Failed!\n");
            return -1;
        }
        printf("读入数据块%d\n", next_read_addr);
    }

    if(write_block_used == 0){
        // 多申请了一个块的输出缓冲 需要将多申请的块释放
        freeBlockInBuffer(write_block, buffer);
    }
    else{
        pushTupleValue(write_block, tuples_number_per_block, 1, next_write_addr+1);
        if (writeBlockToDisk(write_block, next_write_addr, buffer) != 0){
            perror("Writing Block Failed!\n");
            return -1;
        }
    }

    printf("注：结果从磁盘的第%u块开始写入\n\n", projection_ans_addr);
    printf(YELLOW"关系R上的A属性满足投影去重的属性值一共%u个\n\n"NONE, res_count);
//    printf(YELLOW"IO读写一共%d次\n"NONE, buffer->numIO);

    return 1;
}

int sortMergeJoin(Buffer *buffer)
{
    unsigned char *r_read_block, *s_read_block, *write_block;
    int write_block_used = 0;
    unsigned int write_next_addr = sort_merge_join_ans_addr;
    write_block = getNewBlockInBuffer(buffer);
    refreshBlockInBuffer(buffer, write_block);
    int rp=0, sp=0, is_found=0;
    unsigned int r_next_addr = r_sort_addr;
    unsigned int s_next_addr = s_sort_addr;
    unsigned int join_count = 0;
    if ((r_read_block = readBlockFromDisk(r_next_addr, buffer)) == NULL) {
        perror("Reading Block Failed!\n");
        return -1;
    }

    if ((s_read_block = readBlockFromDisk(s_next_addr, buffer)) == NULL) {
        perror("Reading Block Failed!\n");
        return -1;
    }

    // 前面已用3块，故缓存还剩5块，则还能存32个元组
    Tuple s[32];
    int s_index = 0;
    //=================!!!!!!!!!!!!!!!!!!!!!!!===================
    while(1){
        // r读取下一块
        if(rp == tuples_number_per_block){
            r_next_addr = getTupleValue(r_read_block, tuples_number_per_block, 1);
            freeBlockInBuffer(r_read_block, buffer);
            // R读完了
            if (r_next_addr == (r_sort_end_addr + 1)){
                freeBlockInBuffer(s_read_block, buffer);
                break;
            }
            if((r_read_block = readBlockFromDisk(r_next_addr, buffer)) == NULL){
                perror("Reading Block Failed!\n");
                return -1;
            }
            rp = 0;
        }

        // s读取下一块
        if(sp == tuples_number_per_block){
            s_next_addr = getTupleValue(s_read_block, tuples_number_per_block, 1);
            freeBlockInBuffer(s_read_block, buffer);
            // R读完了
            if (s_next_addr == (s_sort_end_addr + 1)){
                freeBlockInBuffer(r_read_block, buffer);
                break;
            }
            if((s_read_block = readBlockFromDisk(s_next_addr, buffer)) == NULL){
                perror("Reading Block Failed!\n");
                return -1;
            }
            sp = 0;
        }

        // 获取R和S的元组
        int a, b, c, d;
        a = getTupleValue(r_read_block, rp, 1);
        b = getTupleValue(r_read_block, rp, 2);
        c = getTupleValue(s_read_block, sp, 1);
        d = getTupleValue(s_read_block, sp, 2);

        // 判断R.A和S.C是否相等
        if(a == c){
            is_found = 1;
            if(s_index == 0){
                s[s_index].x = c;
                s[s_index].y = d;
                s_index += 1;
                // 继续寻找所有相等的元组
                unsigned char *tmp_s_read_block;
                if ((tmp_s_read_block = readBlockFromDisk(s_next_addr, buffer)) == NULL) {
                    perror("Reading Block Failed!\n");
                    return -1;
                }
                int tmp_sp = sp + 1;
                while(1){
                    if(tmp_sp == tuples_number_per_block){
                        unsigned int tmp_s_next_addr;
                        tmp_s_next_addr  = getTupleValue(tmp_s_read_block, tmp_sp, 1);
                        freeBlockInBuffer(tmp_s_read_block, buffer);
                        if(tmp_s_next_addr == s_sort_end_addr){
                            break;
                        }
                        if((tmp_s_read_block = readBlockFromDisk(tmp_s_next_addr, buffer)) == NULL){
                            perror("Reading Block Failed!\n");
                            return -1;
                        }
                        tmp_sp = 0;
                    }
                    int cc, dd;
                    cc = getTupleValue(tmp_s_read_block, tmp_sp, 1);
                    dd = getTupleValue(tmp_s_read_block, tmp_sp, 2);
                    if(cc == c){
                        s[s_index].x = cc;
                        s[s_index].y = dd;
                        s_index += 1;
                        tmp_sp += 1;
                    }
                    else{
                        freeBlockInBuffer(tmp_s_read_block, buffer);
                        break;
                    }
                }
            }
            for(int i=0; i < s_index; i++){
                join_count += 1;
                pushTupleValue(write_block, write_block_used, 1, s[i].x);
                pushTupleValue(write_block, write_block_used, 2, s[i].y);
                write_block_used ++ ;
                if(write_block_used == tuples_number_per_block){
                    pushTupleValue(write_block, tuples_number_per_block, 1, write_next_addr+1);
                    if (writeBlockToDisk(write_block, write_next_addr, buffer) != 0){
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("结果写入磁盘%u\n", write_next_addr);
                    write_next_addr += 1;
                    write_block_used = 0;
                    write_block = getNewBlockInBuffer(buffer);
                    refreshBlockInBuffer(buffer, write_block);
                }
                pushTupleValue(write_block, write_block_used,1, a);
                pushTupleValue(write_block,write_block_used,2, b);
                write_block_used ++ ;
                if(write_block_used == tuples_number_per_block){
                    pushTupleValue(write_block, tuples_number_per_block, 1, write_next_addr+1);
                    if (writeBlockToDisk(write_block, write_next_addr, buffer) != 0){
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("结果写入磁盘%u\n", write_next_addr);
                    write_next_addr += 1;
                    write_block_used = 0;
                    write_block = getNewBlockInBuffer(buffer);
                    refreshBlockInBuffer(buffer, write_block);
                }
            }
            rp++;
        }
            // 不相等则值小的那个对应搜索指针加1
        else{
            s_index = 0;
            if(a < c){
                rp++;
            }
            else if(a > c){
                sp++;
            }
        }
    }
    if(write_block_used == 0){
        freeBlockInBuffer(write_block, buffer);
        // 没找到
        if(is_found == 0){
            return 1;
        }
        else{
            printf("总共连接%d次", join_count);
            return 0;
        }
    }
    else{
        pushTupleValue(write_block, tuples_number_per_block, 1, write_next_addr);
        if (writeBlockToDisk(write_block, write_next_addr, buffer) != 0){
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("结果写入磁盘%u\n", write_next_addr);
        printf(YELLOW"总共连接%d次\n"NONE, join_count);
        return 0;
    }
}

int intersection(Buffer *buffer){
    unsigned char *r_read_block, *s_read_block, *write_block;
    int write_block_used = 0;
    unsigned int write_next_addr = intersection_ans_addr;
    write_block = getNewBlockInBuffer(buffer);
    refreshBlockInBuffer(buffer, write_block);
    int rp=0, sp=0, is_found=0;
    unsigned int r_next_addr = r_sort_addr;
    unsigned int s_next_addr = s_sort_addr;
    unsigned int intersection_count = 0;
    if ((r_read_block = readBlockFromDisk(r_next_addr, buffer)) == NULL) {
        perror("Reading Block Failed!\n");
        return -1;
    }
    if ((s_read_block = readBlockFromDisk(s_next_addr, buffer)) == NULL) {
        perror("Reading Block Failed!\n");
        return -1;
    }

    while(1){
        // r读取下一块
        if(rp == tuples_number_per_block){
            r_next_addr = getTupleValue(r_read_block, tuples_number_per_block, 1);
            freeBlockInBuffer(r_read_block, buffer);
            // R读完了
            if (r_next_addr == (r_sort_end_addr + 1)){
                freeBlockInBuffer(s_read_block, buffer);
                break;
            }
            if((r_read_block = readBlockFromDisk(r_next_addr, buffer)) == NULL){
                perror("Reading Block Failed!\n");
                return -1;
            }
            rp = 0;
        }

        // s读取下一块
        if(sp == tuples_number_per_block){
            s_next_addr = getTupleValue(s_read_block, tuples_number_per_block, 1);
            freeBlockInBuffer(s_read_block, buffer);
            // S读完了
            if (s_next_addr == (s_sort_end_addr + 1)){
                freeBlockInBuffer(s_read_block, buffer);
                break;
            }
            if((s_read_block = readBlockFromDisk(s_next_addr, buffer)) == NULL){
                perror("Reading Block Failed!\n");
                return -1;
            }
            sp = 0;
        }

        // 获取R和S的元组
        int a, b, c, d;
        a = getTupleValue(r_read_block, rp, 1);
        b = getTupleValue(r_read_block, rp, 2);
        c = getTupleValue(s_read_block, sp, 1);
        d = getTupleValue(s_read_block, sp, 2);
        unsigned int tmp_sp = sp;
        unsigned int tmp_s_next_addr = s_next_addr;
        unsigned char *tmp_s_read_block;
        if((tmp_s_read_block = readBlockFromDisk(tmp_s_next_addr, buffer)) == NULL){
            perror("Reading Block Failed!\n");
            return -1;
        }
        // 先判断排序好的属性A和C是否相等，需要将当前R.A和所有相等的S.C逐个判断B和D
        while(a == c){
            // 在判断B和D是否相等
            if (b == d) {
                printf("( X=%d, Y=%d )\n", a, b);
                is_found = 1;
                intersection_count += 1;
                pushTupleValue(write_block, write_block_used, 1, a);
                pushTupleValue(write_block, write_block_used, 2, b);
                write_block_used++;
                if (write_block_used == tuples_number_per_block) {
                    pushTupleValue(write_block, tuples_number_per_block, 1, write_next_addr + 1);
                    if (writeBlockToDisk(write_block, write_next_addr, buffer) != 0) {
                        perror("Writing Block Failed!\n");
                        return -1;
                    }
                    printf("结果写入磁盘%u\n", write_next_addr);
                    write_next_addr += 1;
                    write_block_used = 0;
                    write_block = getNewBlockInBuffer(buffer);
                    refreshBlockInBuffer(buffer, write_block);
                }
                rp += 1;
                tmp_sp = sp;
                break;
            } else{
                tmp_sp += 1;
            }
            if(tmp_sp == tuples_number_per_block){
                tmp_s_next_addr = getTupleValue(s_read_block, tuples_number_per_block, 1);
                freeBlockInBuffer(tmp_s_read_block, buffer);
                // S读完了
                if (tmp_s_next_addr == (s_sort_end_addr + 1)){
                    //freeBlockInBuffer(tmp_s_read_block, buffer);
                    break;
                }
                if((tmp_s_read_block = readBlockFromDisk(tmp_s_next_addr, buffer)) == NULL){
                    perror("Reading Block Failed!\n");
                    return -1;
                }
                tmp_sp = 0;
            }
            c = getTupleValue(tmp_s_read_block, tmp_sp, 1);
            d = getTupleValue(tmp_s_read_block, tmp_sp, 2);
        }
        freeBlockInBuffer(tmp_s_read_block, buffer);
        if(a != c){
            if(a < c){
                rp ++;
            }else{
                sp ++;
            }
        }
    }

    if(write_block_used == 0){
        freeBlockInBuffer(write_block, buffer);
        // 没找到
        if(is_found == 0){
            return 1;
        }
        else{
            printf("R和S的交集有%d个", intersection_count);
            return 0;
        }
    }
    else{
        pushTupleValue(write_block, tuples_number_per_block, 1, write_next_addr);
        if (writeBlockToDisk(write_block, write_next_addr, buffer) != 0){
            perror("Writing Block Failed!\n");
            return -1;
        }
        printf("结果写入磁盘%u\n", write_next_addr);
        printf(YELLOW"R和S的交集有%d个\n"NONE, intersection_count);
        return 0;
    }
}

void outputSortedRelation(Buffer *buffer, unsigned int relation_addr)
{
    unsigned int end_addr = relation_addr == r_sort_addr ? r_sort_end_addr : s_sort_end_addr;
    unsigned int next_addr = relation_addr;
    unsigned char *read_block;
    unsigned int x, y;
    while(next_addr <= end_addr){
        if((read_block = readBlockFromDisk(next_addr, buffer)) == NULL){
            perror("Reading Block Failed!\n");
            return ;
        }
        for(int i = 0; i < tuples_number_per_block; i++){
            x = getTupleValue(read_block, i, 1);
            y = getTupleValue(read_block, i, 2);
            printf("(%u,%u) ", x, y);
        }
        printf("\n");
        next_addr = getTupleValue(read_block, tuples_number_per_block, 1);
        freeBlockInBuffer(read_block, buffer);
    }
}