/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version beta-1.5
 */ 

#include <stdio.h>
#include <stdint.h>

#include "messages.h"
#include "data.h"
#include "setup.h"

int main(int argc,char** argv)
{
	int optind=0;
	int err;
	char separator=',';
	
	//Handle CLI switches, print help, GNU Notice...
	err=handleStartup(argc,argv,&optind,&separator);
	if(err!=0)
		return err;
	
	FILE* fileIn;
	FILE* fileOut;
	
	//Open input and output files
	err=openFiles(argc,argv,optind,&fileIn,&fileOut);
	if(err!=0)
		return err;
	
	long double start,stop;
	getStartStop(fileIn,&start,&stop);
	
	skipHeader(fileIn);
	convertData(fileIn,fileOut,separator,start,getDtheta(fileIn, &start,&stop));
	
	//Close input and output files
	fclose(fileIn);
	fclose(fileOut);
	return 0;
}
