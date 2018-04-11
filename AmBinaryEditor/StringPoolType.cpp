#include <cstring>
#include "StringPoolType.h"
#include "ResChunk.h"

bool StringPoolType::read(uchar **p){
    uchar *origin = *p;
    uchar *pre = *p;
    if(*p == NULL) {
        LOGD("GET StringPool FAILED !");
        return false;
    }
    //read StringPool_head
    void* head = &Stp_header;
    ptr_read((uchar *)head,sizeof(ResStringPool_header),1,p);
    if (Stp_header.header.headerSize != (*p - pre)){
        LOGE("read StringPool_head failed!!");
        return false;
    }
    pre = *p;

    //read string
    if(Stp_header.stringCount > 0){
        //stringoffsets = (uint *)(*p);
        stringoffsets = new uint [Stp_header.stringCount + 2];     //+2是为了给后期插入Application-name时预留的空间
        for (int j = 0; j < Stp_header.stringCount; ++j) {
            stringoffsets[j] = getNext_4(p);
        }
        //(*p) += sizeof(uint)*Stp_header.stringCount;
        if (sizeof(uint)*Stp_header.stringCount != (*p - pre)){
            LOGE("read StringPool_stringoffsets failed!!");
            return false;
        }
        pre = *p;
        //check string_index
        for (int k = 0; k <Stp_header.stringCount-1 ; ++k) {
            if(stringoffsets[k] > stringoffsets[k+1]){
                offsets_fix++;
            }
        }
        if (Stp_header.flags != UTF8_FLAG){
            stringdata_u16 = new ResStringPool_str_u16[Stp_header.stringCount + 2];
            int x = 0;
            for (; x <Stp_header.stringCount - offsets_fix; ++x) {
                //cout<<"location["<<i<<"]: "<<*p - origin<<endl;
                stringdata_u16[x].u16len = getNext_2(p);
                stringdata_u16[x].data = (ushort *)*p;
                (*p) += 2*(stringdata_u16[x].u16len+1);
                //cout<<"location["<<i<<"]: "<<*p - origin<<endl;
            }
#ifdef DEBUG
            LOGSTR(Stp_header.stringCount - offsets_fix,stringdata_u16);
#endif
        }else{
            stringdata_u8 = new ResStringPool_str_u8[Stp_header.stringCount + 2];
            for (int i = 0; i < Stp_header.stringCount - offsets_fix; ++i) {
                stringdata_u8[i].u16len = getNext(p);
                stringdata_u8[i].u8len = getNext(p);
                stringdata_u8[i].data = (char *)*p;
                (*p) += ((int)stringdata_u8[i].u16len + 1);
            }
#ifdef DEBUG
            LOGSTR(Stp_header.stringCount - offsets_fix,stringdata_u8);
#endif
        }
    } else{
        LOGD("No StringPool_string find!");
    }

    //read style
    if(Stp_header.styleCount > 0){
        styleoffsets = (uint *)(*p);
        (*p) += sizeof(uint)*Stp_header.styleCount;
        if (sizeof(uint)*Stp_header.styleCount != (*p - pre)){
            LOGE("read StringPool_styleoffsets failed!!");
            return false;
        }
    } else{
        LOGD("No StringPool_style find!");
    }

    //check size
    long fix = *p - origin;
    content_fix = ALIGN4(fix);
    content_fix = content_fix -fix;
    for (int l = 0; l < content_fix; ++l) {
        (*p)++;

    }
    if(*p - origin != Stp_header.header.size){
        LOGE("read StringPool failed!");
        return false;
    }

    return true;
};

void StringPoolType::write(FILE *fd) {
//    check StringPool
    int size = 0;
//    write Stp_pool header header
    fwrite(&Stp_header, Stp_header.header.headerSize,1,fd);
    size +=  Stp_header.header.headerSize;

    if(Stp_header.stringCount > 0 ){
        for (int i = 0; i < Stp_header.stringCount; ++i) {
            fwrite(&stringoffsets[i],sizeof(uint),1,fd);
            size += sizeof(uint);
        }
    }

//    write StringData
    if(Stp_header.stringCount > 0 ){
        if (Stp_header.flags != UTF8_FLAG){
            for (int i = 0; i < Stp_header.stringCount- offsets_fix; ++i) {
                fwrite(&stringdata_u16[i].u16len, sizeof(ushort), 1,fd);
                size += sizeof(ushort);
                ushort *p = stringdata_u16[i].data;
                for (int j = 0; j <stringdata_u16[i].u16len+1 ; ++j) {
                    fwrite(p,2,1,fd);
                    size += 2;
                    p ++;
                }
            }
        }
        else if(Stp_header.flags == UTF8_FLAG){
            for (int i = 0; i < Stp_header.stringCount - offsets_fix; ++i) {
                fwrite(&stringdata_u8[i].u16len, sizeof(uchar), 1,fd);
                size += sizeof(uchar);
                fwrite(&stringdata_u8[i].u8len, sizeof(uchar), 1,fd);
                size += sizeof(uchar);
                char *p = stringdata_u8[i].data;
                for (int j = 0; j <stringdata_u8[i].u8len+1 ; ++j) {
                    fwrite(p,1,1,fd);
                    size += 1;
                    p ++;
                }
            }
        }
        else{
            LOGE("can't find strPool flag !");
            exit(-1);
        }
    }

//    write StyleOffsets
    if(Stp_header.styleCount > 0 ){
        for (int i = 0; i < Stp_header.styleCount; ++i) {
            fwrite(&stringoffsets[i],sizeof(uint),1,fd);
            size += sizeof(uint);
        }
    }

    int fix = ALIGN4(size);
    uchar fixdata = 0x00;
    for (int k = 0; k < fix-size; ++k) {
        fwrite(&fixdata, 1, 1,fd);
    }
    if (fix != Stp_header.header.size){
        LOGE("StringPool write failed！");
        exit(-1);
    }
}

uint StringPoolType::getStringPoolSize(StringPoolType *p) {
    return p->Stp_header.header.size;
}

uint StringPoolType::getStringPoolCount() {
    return (this->Stp_header.stringCount - this->offsets_fix);
}

uint StringPoolType::getStringPoolFlag(){
    return this->Stp_header.flags;
}

uint StringPoolType::getindex(const char *tag) {
    if(stringdata_u16){
        for (uint i = 0; i < Stp_header.stringCount; ++i) {
            char* content = new char[stringdata_u16[i].u16len+1];
            memset(content, 0, stringdata_u16[i].u16len+1);
            if(!strcmp(tag, ushort2char(stringdata_u16[i].data, content, stringdata_u16[i].u16len))){
                //cout<<"content["<<i<<"]: "<<content<<"    len: "<<stringdata_u16[i].u16len<<endl;
                delete [] content;
                return i;
            }
            //cout<<"content["<<i<<"]: "<<content<<"    len: "<<stringdata_u16[i].u16len<<endl;
            delete [] content;
        }
        return 0xffffffff;
    }
    else if (stringdata_u8){
        for (uint j = 0; j < Stp_header.stringCount; ++j) {
            if(!strcmp(tag,stringdata_u8[j].data)){
                return j;
            }
        }
        return 0xffffffff;
    } else{
        LOGE("unsupport unicode!");
        exit(-1);
    }
}

int StringPoolType::updataStringPool(uint index, char *value) {
    if (Stp_header.flags != UTF8_FLAG){
        ushort* content = new ushort[strlen(value)+1];
        memset(content, 0, sizeof(ushort)*(strlen(value)+1));
        for (int j = 0; j <= strlen(value); ++j) {
            content[j] = (ushort)value[j];
        }
        int dif = sizeof(ushort)*((ushort)strlen(value) - stringdata_u16[index].u16len);
        memset(stringdata_u16[index].data, 0, sizeof(ushort)*(stringdata_u16[index].u16len+1));
        for (int i = index+1; i < Stp_header.stringCount; ++i) {
            stringoffsets[i] += dif;
        }
        uint buff = Stp_header.header.size;
        Stp_header.header.size = Stp_header.header.size - (ushort)content_fix + dif;
        Stp_header.header.size = ALIGN4(Stp_header.header.size);
        buff = Stp_header.header.size - buff;
        stringdata_u16[index].u16len = (ushort)(strlen(value));
        stringdata_u16[index].data = content;
        return buff;
    }else{
        char* content = new char[strlen(value)+1];
        memset(content, 0, sizeof(char)*(strlen(value)+1));
        for (int j = 0; j <= strlen(value); ++j) {
            content[j] = value[j];
        }
        int dif = sizeof(char)*((uchar)strlen(value) - stringdata_u8[index].u16len);
        memset(stringdata_u8[index].data, 0, sizeof(char)*(stringdata_u8[index].u16len+1));
        for (int i = index+1; i < Stp_header.stringCount; ++i) {
            stringoffsets[i] += dif;
        }
        uint buff = Stp_header.header.size;
        Stp_header.header.size = Stp_header.header.size - (ushort)content_fix + dif;
        Stp_header.header.size = ALIGN4(Stp_header.header.size);
        buff = Stp_header.header.size - buff;
        stringdata_u8[index].u16len = (uchar)strlen(value);
        stringdata_u8[index].u8len = (uchar)strlen(value);
        stringdata_u8[index].data = content;
        return buff;
    }
}

int StringPoolType::insert_utf8(char *p) {
    int size = 0;
    int index = Stp_header.stringCount - offsets_fix;
    stringoffsets[index] = stringoffsets[index - 1] + stringdata_u8[index - 1].u8len + 3;
    size = size + 4;
    stringdata_u8[index].u8len = (uchar)strlen(p);
    stringdata_u8[index].u16len = (uchar)strlen(p);
    stringdata_u8[index].data = p;
    size = size + 2 + stringdata_u8[index].u16len;
    Stp_header.stringCount++;
    uint buff = Stp_header.header.size;
    Stp_header.header.size = Stp_header.header.size - (ushort)content_fix + size;
    Stp_header.header.size = ALIGN4(Stp_header.header.size);
    buff = Stp_header.header.size - buff;
    return buff;
}

int StringPoolType::insert_utf16(char *p) {
    int size = 0;
    int index = Stp_header.stringCount - offsets_fix;
    stringoffsets[index] = stringoffsets[index - 1] + 2*stringdata_u16[index - 1].u16len + 4;
    Stp_header.stringsStart += 4;
    size = size + 4;
    stringdata_u16[index].u16len = (ushort)strlen(p);
    ushort* content = new ushort[stringdata_u16[index].u16len+1];
    memset(content, 0, sizeof(ushort)*(stringdata_u16[index].u16len+1));
    for (int j = 0; j <= strlen(p); ++j) {
        content[j] = (ushort)p[j];
    }
    stringdata_u16[index].data = content;
    size = size + 4 + 2*stringdata_u16[index].u16len;
    Stp_header.stringCount++;
    uint buff = Stp_header.header.size;
    Stp_header.header.size = Stp_header.header.size - (ushort)content_fix + size;
    Stp_header.header.size = ALIGN4(Stp_header.header.size);
    buff = Stp_header.header.size - buff;
    return buff;
}