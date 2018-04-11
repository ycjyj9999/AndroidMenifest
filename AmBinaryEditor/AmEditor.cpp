//
// Created by liuyichen on 17-12-5.
//
#include "AmEditor.h"
#include "ResChunk.h"
#include "StringPoolType.h"

uchar* AmEditor::open(char* input){
    FILE * pFile;
    long lSize;
    uchar * buffer;
    long result;

    //以二进制格式打开文件
    pFile = fopen (input, "rb" );
    if (pFile == NULL)
    {
        fputs ("File error",stderr);
        exit (0);
    }

    //获取文件大小
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);

    //TODO  ?
    origin_size = lSize;

    rewind (pFile);

    // 分配内存存储整个文件
    buffer = (uchar*) malloc (sizeof(uchar)*lSize);
    if (buffer == NULL)
    {
        fputs ("Memory error",stderr);
        exit (0);
    }

    //将文件拷贝到buffer中
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize)
    {
        fputs ("Reading error",stderr);
        exit (0);
    }
    return buffer;
}

void AmEditor::read(uchar * pc) {
    uchar **f = &pc;
    //read ResChunk
    resHeader.read(f);
    if (origin_size != resHeader.RC_header.size){
        LOGE("Memory_size error");
        exit(-1);
    }
    current_size += resHeader.RC_header.headerSize;

    //read strPool
    if(!strPool.read(f)) {
        LOGE("read strPool failed!");
        exit(-1);
    }
    current_size += strPool.getStringPoolSize(&strPool);

    //read resourceMap
    if(!resMap.read(f)) {
        LOGE("read resMap failed!");
        exit(-1);
    }
    current_size += resMap.getResMapSize(&resMap);

    //read xml
    if(!xml.read(f, (uint)(origin_size-current_size))){
        LOGE("read Xml failed!");
        exit(-1);
    }
}

void AmEditor::write() {
    const char * p = output_path.data();
    FILE* fd = fopen(p,"wb");
    resHeader.write(fd);
    strPool.write(fd);
    resMap.write(fd);
    xml.write(fd);
}

void AmEditor::getApplicationName() {
    char tag[] = "application";
    char att[]="name";
    uint tag_index = strPool.getindex(tag);
    uint att_index = strPool.getindex(att);
    uint count = strPool.getStringPoolCount();
    uint attValue_index = xml.getAttValue(tag_index, att_index, count);
    if (strPool.getStringPoolFlag() == UTF8_FLAG){
        for (int i = 0; i < strPool.stringdata_u8[attValue_index].u8len; ++i) {
            cout<<strPool.stringdata_u8[attValue_index].data[i];
        }
    }
    else {
        for (int i = 0; i < strPool.stringdata_u16[attValue_index].u16len; ++i) {
            cout<<(char)(strPool.stringdata_u16[attValue_index].data[i]);
        }
    }

}

void AmEditor::updateApplication(char *name) {
    char tag[] = "application";
    char att[]="name";
    uint tag_index = strPool.getindex(tag);
    uint att_index = strPool.getindex(att);
    uint count = strPool.getStringPoolCount();
    uint attValue_index = xml.getAttValue(tag_index, att_index, count);
    if (strPool.getStringPoolFlag() == UTF8_FLAG){
        resHeader.RC_header.size += strPool.insert_utf8(name);
    }
    else {
        resHeader.RC_header.size += strPool.insert_utf16(name);
    }
    if (attValue_index != 0xffffffff) {

        resHeader.RC_header.size += strPool.updataStringPool(attValue_index, name);
    }
    else{
        string attcontent[3] = {"theme","icon","label"};
        vector<uint> attindex;
        for (int i = 0; i < 3; ++i) {
            attindex.push_back(strPool.getindex(attcontent[i].c_str()));
        }
        xml.insertAttrib(attindex,tag_index, att_index, count);
        resHeader.RC_header.size += 20;
        if (strPool.getStringPoolFlag() == UTF8_FLAG){
            resHeader.RC_header.size += strPool.insert_utf8(name);
        }
        else {
            resHeader.RC_header.size += strPool.insert_utf16(name);
        }
    }
}