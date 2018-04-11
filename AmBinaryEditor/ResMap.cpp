//
// Created by liuyichen on 17-12-5.
//

#include "ResMap.h"
#include "ResChunk.h"

bool ResourceMap::read(uchar **pc){
    uchar* origin_location = *pc;
    ResChunk_header* head = &Res_header;
    //read head
    ptr_read((uchar*)head,sizeof(ResChunk_header),1,pc);

    //read resids
    resids_num = (Res_header.size-Res_header.headerSize)/4;
    resids = new uint[resids_num];
    if (*pc - origin_location != Res_header.headerSize){
        LOGE("read ResMap_head failed!");
        return false;
    }
    for (int i = 0; i < resids_num ; ++i) {
        ptr_read((uchar*)(resids+i),4,1,pc);
    }
    if (*pc - origin_location != Res_header.size){
        LOGE("read ResMap failed!");
        return false;
    }
    return true;
}

void ResourceMap::write(FILE *fd) {

    int size = 0;
    //write ResChunk_header
    fwrite(&Res_header, sizeof(ResChunk_header), 1, fd);
    size += sizeof(ResChunk_header);

    //write ResChunk_resids
    fwrite(resids, sizeof(uint)*resids_num, 1, fd);
    size += sizeof(uint) * resids_num;
    if(size != Res_header.size)
        LOGE("Read ResChunk_header failed !");
}

uint ResourceMap::getResMapSize(ResourceMap *p) {
    return p->Res_header.size;
}