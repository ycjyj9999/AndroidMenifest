//
// Created by liuyichen on 17-12-12.
//
#include "Namespace.h"
#include "XmlTree.h"
#include "ResChunk.h"

bool Namespace::read(uchar **p, uint pc_size) {
    size = pc_size;
    //read Namespace_start
    ptr_read((uchar*)&NamespaceStart, sizeof(NamespaceType), 1, p);
    current_size += NamespaceStart.header.header.size;

    //read Xml
    Xml_head = new XmlNode();
    current_size += Xml_head->read(p);
    readXml_list(Xml_head, p);

    //read Namespace_end
    ptr_read((uchar*)&NamespaceEnd, sizeof(NamespaceType), 1, p);
    return true;
}

void Namespace::readXml_list(XmlNode *node, uchar **p) {
    if ((getNextType(*p) == RES_XML_END_NAMESPACE_TYPE) && ((size - current_size) == 24)){
        return;
    }
    node->next = new XmlNode();
    current_size += node->next->read(p);
    readXml_list(node->next, p);
    if (current_size > size){
        LOGE("read too much Xml!");
        exit(-1);
    }
}

void Namespace::write(FILE *fd) {
    write(Xml_head,fd);
}

void Namespace::write(XmlNode *boot, FILE *fd) {
    fwrite((uchar*)&NamespaceStart, sizeof(NamespaceType), 1, fd);

    writeXml_list(boot,fd);

    fwrite((uchar*)&NamespaceEnd, sizeof(NamespaceType), 1, fd);
}

void Namespace::writeXml_list(XmlNode *node, FILE *fd) {
    node->write(fd);
    if (node->next){
        writeXml_list(node->next,fd);
    } else{
        return;
    }
}

bool check_XmlNode(XmlNode* node, uint tag_index){
    if (node->flag == 1){
        StartElement* p = (StartElement*)node->data;
        if(p->attrExt.name == tag_index){
            return true;
        }
    }
    return false;
}

XmlNode* find_XmlNode(XmlNode* node,uint tag_index){
    if (check_XmlNode(node, tag_index)){
        return node;
    } else{
        return find_XmlNode(node->next,tag_index);
    }

}

uint Namespace:: getAttValue(uint tag_index, uint att_index, uint count) {
    int i = 0;
    XmlNode* p = find_XmlNode(Xml_head,tag_index);
    StartElement* buf = (StartElement*)p->data;
    for (i = 0; i < buf->attrExt.attributeCount; ++i) {
        if(buf->attrib[i].name == att_index){
            return buf->attrib[i].rawValue;
        }
    }
    if (i == buf->attrExt.attributeCount){

        return 0xffffffff;
    }
    return 0;
}

void Namespace::insertAttrib(vector<uint > attindex, uint tag_index, uint att_index, uint count) {
    int i = 0;
    XmlNode* p = find_XmlNode(Xml_head,tag_index);
    StartElement* buf = (StartElement*)p->data;
    for (uint j = 0; j < buf->attrExt.attributeCount; ++j) {
        for (int k = 0; k < attindex.size(); ++k) {
            if(buf->attrib[j].name == attindex[k]){
                buf->attrib_flag = j+1;
            }
        }
    }
    i = buf->attrExt.attributeCount;
    buf->attrExt.attributeCount++;
    buf->header.header.size += 20;
    buf->attrib[i].ns = NamespaceStart.ext.uri;
    buf->attrib[i].name = att_index;
    buf->attrib[i].rawValue = count;
    buf->attrib[i].typedValue.size = 0x08;
    buf->attrib[i].typedValue.res0 = 0x00;
    buf->attrib[i].typedValue.data = count;
    buf->attrib[i].typedValue.dataType = 0x03;
}


