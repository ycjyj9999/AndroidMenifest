//
// Created by liuyichen on 17-12-5.
//

#ifndef AMBINARYEDITOR_RESMAP_H
#define AMBINARYEDITOR_RESMAP_H

#include "utils.h"
#include "ResChunk.h"



class ResourceMap{
public:
    ResChunk_header Res_header;
    int resids_num;
    uint* resids;
    ResourceMap():resids(NULL){}
    ~ResourceMap(){SAFEDELARRAY(resids);}
    bool read(uchar **p);
    void write(FILE* fd);
    uint getResMapSize(ResourceMap* p);
};

#endif //AMBINARYEDITOR_RESMAP_H
