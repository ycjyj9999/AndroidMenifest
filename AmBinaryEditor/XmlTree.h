//
// Created by liuyichen on 17-12-5.
//

#ifndef AMBINARYEDITOR_XMLTREE_H
#define AMBINARYEDITOR_XMLTREE_H

#include "ResChunk.h"

typedef struct {
    ResChunk_header header;
    uint lineNumber;
    uint comment;
} Element_head;

typedef struct{
    ushort size;
    uchar res0;
    uchar dataType;
    uint data;
}Res_value;

typedef struct{
    uint ns;
    uint name;
    ushort attributeStart;
    ushort attributeSize;
    ushort attributeCount;
    ushort idIndex;
    ushort classIndex;
    ushort styleIndex;
} StartElement_attrExt;

typedef struct {
    uint ns;
    uint name;
} EndElement_Ext;

typedef struct {
    uint ns;  //uint  .index
    uint name;
    uint rawValue;
    Res_value typedValue;
}StartElement_attribute;

typedef struct {
    uint prefix;
    uint uri;
}NamespaceExt;

class XmlNode{
public:
    Element_head header;
    XmlNode* next;
    void* data;
    ushort flag;
    XmlNode():next(NULL),data(NULL),flag(0){}
    ~XmlNode(){
        SAFEDEL(next);
        if(data){free(data);data=NULL;}
    }
    uint read(uchar** p);
    void write(FILE* p);
};

class StartElement: public XmlNode{
public:
    uint attrib_flag;
    StartElement_attrExt attrExt;
    StartElement_attribute* attrib;
    StartElement():attrib(NULL),attrib_flag(0){}
    ~StartElement(){
        SAFEDELARRAY(attrib);
    }
};

class EndElement: public XmlNode{
public:
    EndElement_Ext endEleExt;
};

class DateElement: public XmlNode{
public:
    uchar* cdataExt;
};

class NamespaceElement: public XmlNode{
public:
    EndElement_Ext namespaceExt;
};


#endif //AMBINARYEDITOR_XMLTREE_H
