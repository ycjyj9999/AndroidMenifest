//
// Created by liuyichen on 17-12-5.
//

#ifndef AMBINARYEDITOR_RESCHUNK_H
#define AMBINARYEDITOR_RESCHUNK_H

#include "utils.h"
#include <iostream>
using namespace std;

typedef enum:ushort
{
    RES_NULL_TYPE               = 0x0000,
    RES_STRING_POOL_TYPE        = 0x0001,
    RES_TABLE_TYPE              = 0x0002,
    RES_XML_TYPE                = 0x0003,
    // Chunk types in RES_XML_TYPE
    RES_XML_FIRST_CHUNK_TYPE    = 0x0100,
    RES_XML_START_NAMESPACE_TYPE= 0x0100,//256
    RES_XML_END_NAMESPACE_TYPE  = 0x0101,
    RES_XML_START_ELEMENT_TYPE  = 0x0102,
    RES_XML_END_ELEMENT_TYPE    = 0x0103,
    RES_XML_CDATA_TYPE          = 0x0104,
    RES_XML_LAST_CHUNK_TYPE     = 0x017f,
    // This contains a uint32_t array mapping strings in the string
    // pool back to resource identifiers.  It is optional.
    RES_XML_RESOURCE_MAP_TYPE   = 0x0180,
    // Chunk types in RES_TABLE_TYPE
     RES_TABLE_PACKAGE_TYPE     = 0x0200,
    RES_TABLE_TYPE_TYPE         = 0x0201,
    RES_TABLE_TYPE_SPEC_TYPE    = 0x0202,
    RES_TABLE_LIBRARY_TYPE      = 0x0203
} RES_TYPE;
typedef struct {
    ushort type;  			//"Type identifier for this chunk">;
    ushort headerSize;			//"Size of the chunk header (in bytes)">;
    uint size;
}ResChunk_header;

class ResChunk
{
public:
    ResChunk_header RC_header;				//"Total size of this chunk (in bytes)">;
    void read(uchar ** p);
    void update();
    void write(FILE* p);
};

#endif //AMBINARYEDITOR_RESCHUNK_H
