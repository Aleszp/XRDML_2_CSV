/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version beta-1.7b
 */ 
#include <stdio.h>
#include <stdint.h>

#include "messages.h"
#include "data.h"
#include "setup.h"

/**
* Just the main function
* @param argc - arguments counter
* @param argv - table with arguments values
* @return ==0 for proper execution, !=0 in case of abnotmal execution
*/ 
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
	
	long double start,stop,Dtheta;
	getStartStop(fileIn,&start,&stop);
	
	skipHeader(fileIn);
	Dtheta=getDtheta(fileIn, &start,&stop);
	convertData(fileIn,fileOut,separator,&start,&Dtheta);
	
	//Close input and output files
	fclose(fileIn);
	fclose(fileOut);
	return 0;
}
