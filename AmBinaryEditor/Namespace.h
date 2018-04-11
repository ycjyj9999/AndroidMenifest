//
// Created by liuyichen on 17-12-12.
//

#ifndef NEW_AMBINARYEDITOR_NAMESPACE_H
#define NEW_AMBINARYEDITOR_NAMESPACE_H

#include "XmlTree.h"

typedef struct {
    Element_head header;
    NamespaceExt ext;
} NamespaceType;

class Namespace{
    NamespaceType NamespaceStart;
    XmlNode* Xml_head;
    NamespaceType NamespaceEnd;
    uint current_size;
    uint size;
public:
    Namespace():Xml_head(NULL),current_size(0){}
    Namespace(ushort a):Xml_head(NULL){}
    ~Namespace(){
        SAFEDEL(Xml_head);
    }
    bool read(uchar **, uint size);
    void readXml_list(XmlNode* node,uchar** p);
    void write(FILE* fd);
    void write(XmlNode* boot,FILE* fd);
    void writeXml_list(XmlNode* node,FILE* fd);
    uint getAttValue(uint tag_index, uint att_index, uint count);
    void insertAttrib(vector<uint > attindex,uint tag_index, uint att_index, uint count);
};

#endif //NEW_AMBINARYEDITOR_NAMESPACE_H
