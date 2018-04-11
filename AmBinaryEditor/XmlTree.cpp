//
// Created by liuyichen on 17-12-5.
//

#include "XmlTree.h"

uint XmlNode::read(uchar **p) {
    uchar* origin = *p;
    uint size = 0;
    switch (getNextType(*p)){
        case RES_XML_START_ELEMENT_TYPE:{
            flag = 1;
            data = new StartElement();
            StartElement* buff = (StartElement*)data;
            ptr_read((uchar*)&buff->header, sizeof(Element_head), 1, p);
            size = buff->header.header.size;
            ptr_read((uchar*)&buff->attrExt, sizeof(StartElement_attrExt), 1, p);
            buff->attrib = new StartElement_attribute[buff->attrExt.attributeCount + 2];
            for (int i = 0; i < buff->attrExt.attributeCount; ++i) {
                ptr_read((uchar*)&buff->attrib[i], sizeof(StartElement_attribute), 1, p);
            }
            //cout<<"READ_START_ELEMENT_TYPE: "<<data<<endl;
            break;
        }
        case RES_XML_END_ELEMENT_TYPE:{
            flag = 2;
            data = new EndElement();
            EndElement* buff = (EndElement*)data;
            ptr_read((uchar*)&buff->header, sizeof(Element_head), 1, p);
            size = buff->header.header.size;
            ptr_read((uchar*)&buff->endEleExt, sizeof(EndElement_Ext), 1, p);
            //cout<<"READ_END_ELEMENT_TYPE: "<<data<<endl;
            break;
        }
        case RES_XML_CDATA_TYPE:{
            flag = 3;
            data = new DateElement();
            DateElement* buff = (DateElement*)data;
            ptr_read((uchar*)&buff->header, sizeof(Element_head), 1, p);
            size = buff->header.header.size;
            buff->cdataExt = new uchar [buff->header.header.size - buff->header.header.headerSize];
            ptr_read(buff->cdataExt, buff->header.header.size - buff->header.header.headerSize, 1, p);
            //cout<<"READ_CDATA_ELEMENT_TYPE: "<<data<<endl;
            break;
        }
        case RES_XML_START_NAMESPACE_TYPE:
        case RES_XML_END_NAMESPACE_TYPE:{
            flag = 4;
            data = new NamespaceElement();
            NamespaceElement* buff = (NamespaceElement*)data;
            ptr_read((uchar*)&buff->header, sizeof(Element_head), 1, p);
            size = buff->header.header.size;
            ptr_read((uchar*)&buff->namespaceExt, sizeof(EndElement_Ext), 1, p);
            break;
        }
        default:{
            LOGE(getNextType(*p));
            return false;
        }
    }
    if ((*p-origin) != size) {
        LOGE("read Xmlnode failed!");
        exit(-1);
    }
    return size;
}

void XmlNode::write(FILE *fd) {
    switch (getNextType((uchar*)data)){
        case RES_XML_START_ELEMENT_TYPE:{
            //cout<<"WRITE_START_ELEMENT_TYPE: "<<data<<endl;
            StartElement* buff = (StartElement*)data;
            fwrite((uchar*)&buff->header, sizeof(Element_head), 1, fd);
            fwrite((uchar*)&buff->attrExt, sizeof(StartElement_attrExt), 1, fd);
            for (int i = 0; i < buff->attrExt.attributeCount; ++i) {
                if((buff->attrib_flag != 0)&&(i == buff->attrib_flag)){
                    fwrite((uchar*)&buff->attrib[buff->attrExt.attributeCount-1],sizeof(StartElement_attribute),1,fd);
                    buff->attrExt.attributeCount--;
                    buff->attrib_flag = 0;
                }
                fwrite((uchar*)&buff->attrib[i],sizeof(StartElement_attribute),1,fd);
            }
            break;
        }
        case RES_XML_END_ELEMENT_TYPE:{
            //cout<<"WRITE_END_ELEMENT_TYPE: "<<data<<endl;
            EndElement* buff = (EndElement*)data;
            fwrite((uchar*)&buff->header, sizeof(Element_head), 1, fd);
            fwrite((uchar*)&buff->endEleExt, sizeof(EndElement_Ext), 1, fd);
            break;
        }
        case RES_XML_CDATA_TYPE:{
            //cout<<"WRITE_CDATA_ELEMENT_TYPE: "<<data<<endl;
            DateElement* buff = (DateElement*)data;
            fwrite((uchar*)&buff->header, sizeof(Element_head), 1, fd);
            fwrite(buff->cdataExt, buff->header.header.size - buff->header.header.headerSize, 1, fd);
            break;
        }
        case RES_XML_START_NAMESPACE_TYPE:
        case RES_XML_END_NAMESPACE_TYPE:{
            NamespaceElement* buff = (NamespaceElement*)data;
            fwrite((uchar*)&buff->header, sizeof(Element_head), 1, fd);
            fwrite((uchar*)&buff->namespaceExt, sizeof(EndElement_Ext), 1, fd);
            break;
        }
        default:{
            LOGE(getNextType((uchar*)data));
            exit(-1);
        }
    }
}