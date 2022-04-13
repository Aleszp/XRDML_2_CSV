/*
 * Part Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 

#ifndef _FILE_H_
#define _FILE_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif

#include <stdint.h>

int getStartStop(FILE* fileIn,long double* start,long double* stop);
int skipHeader(FILE* fileIn);
uint64_t countAngles(FILE* fileIn);
long double getDtheta(FILE* fileIn, long double* start,long double* stop);
void convertData(FILE* fileIn,FILE* fileOut,char separator,long double* start,long double* Dtheta);

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//_FILE_H_
