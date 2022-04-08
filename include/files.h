#ifndef _FILE_H_
#define _FILE_H_
#include <stdint.h>

int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut);
void getStartStop(FILE* fileIn,long double* start,long double* stop);
void skipHeader(FILE* fileIn);
uint64_t countAngles(FILE* fileIn);


#endif
