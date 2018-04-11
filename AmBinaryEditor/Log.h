/*
* @Author: 浅浅徘徊
* @Date:   2017-04-26 20:53:20
* @Last Modified by:   浅浅徘徊
* @Last Modified time: 2017-04-26 20:53:20
*/

#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>

#define TODO()  std::cout<<"[Need to fill your code!]\033[31m "<<std::endl<<"        "<<__FILE__<<"\n        "<<__LINE__<<"\n\033[0m"
#define LOGE(str)  std::cout<<"[Error]\033[31m "<<str<<std::endl<<"        "<<__FILE__<<"\n        "<<__LINE__<<"\n\033[0m"

#ifdef DEBUG
    #define LOGD(str)  std::cout<<"[Debug]\033[32m "<<str<<"\n\033[0m"
#define LOGSTR(len,p) \
do{ \
    for (int j = 0; j <len; ++j) { \
        cout<<"content["<<j<<"]: "; \
        int i = 0; \
        for (i = 0; i <p[j].u16len ; ++i) { \
            cout<<(char)p[j].data[i]; \
        } \
        cout<<endl; \
        cout<<"len["<<j<<"]: "<<p[j].u16len<<endl; \
        cout<<"str["<<j<<"]: "<<*(p[j].data)<<endl; \
    } \
} \
while(0)


#else
    #define LOGD(str) {}
#endif

#ifdef INFO
    #define LOGI(str)  std::cout<<"[Infor] "<<str<<"\n"
#else
    #define LOGI(str) {}
#endif

#define SAFEDEL(ptr) if(ptr){delete ptr; ptr = NULL;}
#define SAFEDELARRAY(ptr) if(ptr){delete [] ptr;ptr = NULL;}

#endif