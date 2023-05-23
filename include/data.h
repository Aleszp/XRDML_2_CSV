/*
 * Part of program XRDML_2_CSV
 * Data oriented functions header file.
 *
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

		int getStartStop(FILE* fileIn,long double* start,uint64_t* count,long* offset);
		int skipHeader(FILE* fileIn);
		uint64_t countAngles(FILE* fileIn);
		long double getDtheta(FILE* fileIn, long double* start,uint64_t* count);
		int calculateAngles(long double* start,uint64_t* count,long double* Dtheta);
		void convertData1(FILE* fileIn,FILE* fileOut,char separator,long double* start,long double* Dtheta);
		void convertData2(FILE* fileIn,FILE* fileOut,char separator,long double* start,long offset);
	#ifdef __cplusplus
	} //end of extern "C"
	#endif

#endif	//_FILE_H_
