//
// Created by liuyichen on 17-12-5.
//
#include "ResChunk.h"

uchar getNext(uchar **pc){
    uchar result = **pc;
    (*pc)++;
    return result;
}

ushort getNextType(uchar* p){
    ushort result = *(ushort*)p;
    return result;
}

ushort getNext_2(uchar **pc){
    union Bu2 v;
    ushort *p = (ushort *)*pc;
    v.value = *p++;
    (*pc) += 2;
    return v.value;
}

uint getNext_4(uchar **pc){
    union Bu4 v;
    uint *p = (uint *)*pc;
    v.value = *p;
    (*pc) += 4;
    return v.value;
}

void ptr_read(uchar *target,int size,int num,uchar **ptr){
    int i=0;
    int j=0;
    for (; i<num ; ++i){
        for (; j<size; ++j){
            *target = **ptr;
            target++;
            (*ptr)++;
        }
    }
}

char* ushort2char(ushort* target,char* result,ushort len){
    for (int i = 0; i < len; ++i) {
        result[i] = (char)target[i];
    }
    return result;
}