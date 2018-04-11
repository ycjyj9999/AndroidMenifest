//
// Created by liuyichen on 17-12-5.
//
#include "AmEditor.h"



int main(int argc, char const *argv[])
{

    AmEditor manifest(argv[1],argv[2]);
    uchar * pc = manifest.open((char*)argv[1]);
    manifest.read(pc);
    char application_name[]= "com.payegis.FirstApplication";
    //manifest.updateApplication(application_name);
    manifest.getApplicationName();
    //manifest.write();
    free(pc);
	return 0;
}