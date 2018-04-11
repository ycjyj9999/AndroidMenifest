//
// Created by liuyichen on 17-12-5.
//

#ifndef NEW_AMBINARYEDITOR_AMEDITOR_H
#define NEW_AMBINARYEDITOR_AMEDITOR_H

#include <iostream>
#include "Log.h"
#include "utils.h"
using namespace std;

#include "ResMap.h"
#include "StringPoolType.h"
#include "XmlTree.h"
#include "Namespace.h"

class AmEditor{
private:
    string input_path;
    string output_path;
    ResChunk resHeader;
    StringPoolType strPool;
    ResourceMap resMap;
    Namespace xml;
    long origin_size;
    long current_size;
public:
    AmEditor(const char* input,const char* output):input_path(input),output_path(output),origin_size(0),current_size(0),resHeader(),strPool(){}
    uchar * open(char* argv);
    void read(uchar * pc);
    void write();
    void getApplicationName();
    void updateApplication(char* name);
};
#endif //NEW_AMBINARYEDITOR_AMEDITOR_H
