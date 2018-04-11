//
// Created by liuyichen on 17-12-5.
//
#include "ResChunk.h"

void ResChunk::read(uchar **p) {

    if(*p == NULL) {
        LOGD("GET RESCHUNK_HEADER FAILED !");
        exit(0);
    }

    RC_header.type = getNext_2(p);
    RC_header.headerSize = getNext_2(p);
    RC_header.size = getNext_4(p);
}

void ResChunk::write(FILE *fd) {

    fwrite(&RC_header,sizeof(RC_header),1,fd);
    if(ftell(fd) != RC_header.headerSize){
        LOGE("write RC_header failed !");
    }

}
