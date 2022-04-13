/*
 * Part Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 
#ifndef _ERROR_H_
#define _ERROR_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif
/**
 * Error codes for various functions.
 */ 
enum errors{OTHER=-2,HELP=-1,OK=0,ARGUMENTS=1,INPUT=2,OUTPUT=3,EMPTY=4,NOINTENSITIES=5};

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//end of _ERROR_H_
