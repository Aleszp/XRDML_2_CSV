/*
 * Part Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 
 
#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif

void GPLnotice();
void printHelp();
void wrongUsage();
void wrongFile(char* mode,char* path);

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//end of _MESSAGES_H_
