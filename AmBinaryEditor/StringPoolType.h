//
// Created by liuyichen on 17-12-5.
//

#ifndef AMBINARYEDITOR_STRINGPOOLTYPE_H
#define AMBINARYEDITOR_STRINGPOOLTYPE_H

#include "ResChunk.h"
#include <iostream>
#include "utils.h"

using namespace std;

#define UTF8_FLAG 0x00100
#define SORTED_FLAG 1<<0

typedef struct{
    ushort u16len;
    ushort * data;
}ResStringPool_str_u16;

typedef struct{
    uchar u16len;
    uchar u8len;
    char* data;
}ResStringPool_str_u8;

typedef struct{
    ushort u16len;
    string data;
}ResStringPool_str;

typedef struct{

}ResStringPool_sty;

typedef struct
{
    ResChunk_header header;
    uint stringCount;				//"Number of strings in this pool">;//  (number of uint32_t indices that follow in the data).
    uint styleCount;				//"Number of style span arrays in the pool">;// (number of uint32_t indices follow the string indices).
    uint flags;						//"Flags">;
    uint stringsStart;				//"Index from header of the string data">;
    uint stylesStart;				//"Index from header of the style data">;
} ResStringPool_header;

class StringPoolType{
public:
    ushort offsets_fix;
    long content_fix;
    uint* stringoffsets;
    uint* styleoffsets;
    ResStringPool_header Stp_header;
    ResStringPool_str_u16* stringdata_u16;
    ResStringPool_str_u8* stringdata_u8;
    //ResStringPool_str* stringdata_content;
    ResStringPool_sty* styledata;

    StringPoolType():offsets_fix(0),stringoffsets(NULL),styleoffsets(NULL),stringdata_u16(NULL),stringdata_u8(NULL),styledata(NULL),Stp_header(){}
    ~StringPoolType(){
        //SAFEDELARRAY(stringoffsets);
        //SAFEDELARRAY(styleoffsets);
        SAFEDELARRAY(stringdata_u16);
        //SAFEDELARRAY(stringdata_content);
        SAFEDELARRAY(stringdata_u8);
        //SAFEDELARRAY(styledata);
    };
    bool read(uchar ** p);
    void update(char *tag);
    uint getindex(const char *tag);
    void write(FILE* p);
    uint getStringPoolSize(StringPoolType* p);
    int updataStringPool(uint index, char* value);
    uint getStringPoolCount();
    uint getStringPoolFlag();
    int insert_utf8(char* p);
    int insert_utf16(char* p);
};
#endif //AMBINARYEDITOR_STRINGPOOLTYPE_H
