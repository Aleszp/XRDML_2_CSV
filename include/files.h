#ifndef _FILE_H_
#define _FILE_H_
#include <stdint.h>

int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut);

uint64_t countAngles(FILE* fileIn);


#endif
