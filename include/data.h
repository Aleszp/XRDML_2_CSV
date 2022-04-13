#ifndef _FILE_H_
#define _FILE_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif

#include <stdint.h>

void getStartStop(FILE* fileIn,long double* start,long double* stop);
void skipHeader(FILE* fileIn);
uint64_t countAngles(FILE* fileIn);
long double getDtheta(FILE* fileIn, long double* start,long double* stop);
void convertData(FILE* fileIn,FILE* fileOut,char separator,long double* start,long double* Dtheta);

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//_FILE_H_
