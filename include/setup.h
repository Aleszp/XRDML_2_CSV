/*
 * Part Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 
 
#ifndef _SETUP_H_
#define _SETUP_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif

int handleStartup(int argc,char** argv,int* optind_,char* separator);
int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut);
char selectSeparator(int argc,char** argv,int optind);
int exitProgram(FILE** fileIn, FILE** fileOut,int errorCode);

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//_SETUP_H_
