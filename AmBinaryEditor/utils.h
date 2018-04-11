//
// Created by liuyichen on 17-12-5.
//

#ifndef NEW_AMBINARYEDITOR_UTILS_H
#define NEW_AMBINARYEDITOR_UTILS_H

#define ALIGN4(num) (num +3)&(~3)

#include <string>
#include <zconf.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "Log.h"
using namespace std;

#define uchar unsigned char

union Bu2{
    uchar array[2];
    ushort value;
};

union Bu4{
    uchar array[4];
    uint value;
};

uchar getNext(uchar **pc);
ushort getNextType(uchar* p);
ushort getNext_2(uchar **pc);
uint getNext_4(uchar **pc);
void ptr_read(uchar *target,int size,int num,uchar **ptr);
char* ushort2char(ushort* target,char* result,ushort len);

#endif //NEW_AMBINARYEDITOR_UTILS_H
