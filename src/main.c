/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version beta-1.9b
 */ 
#include <stdio.h>
#include <stdint.h>

#include "messages.h"
#include "data.h"
#include "setup.h"
#include "error.h"

/**
* Just the main function
* @param argc - arguments counter
* @param argv - table with arguments values
* @return ==0 for proper execution, !=0 in case of abnormal execution
*/ 
int main(int argc,char** argv)
{
	int optind=0;
	int err;
	char separator=',';
	
	//Handle CLI switches, print help, GNU Notice...
	err=handleStartup(argc,argv,&optind,&separator);
	if(err!=OK)
		return err;
	
	FILE* fileIn;
	FILE* fileOut;
	
	//Open input and output files
	err=openFiles(argc,argv,optind,&fileIn,&fileOut);
	if(err!=OK)
		return err;
	
	long double start,stop,Dtheta;
	err=getStartStop(fileIn,&start,&stop);
	if(err!=OK)
	{
		fprintf(stderr,"Could not get start or stop angle. Is %s proper input file?\n",argv[argc-optind]);
		return err;
	}
	
	skipHeader(fileIn);
	Dtheta=getDtheta(fileIn, &start,&stop);
	if(Dtheta>0.0)	//if data is reasonable - convert data
	{
		convertData(fileIn,fileOut,separator,&start,&Dtheta);
		//Close input and output files
		fclose(fileIn);
		fclose(fileOut);
		return OK;
	}

	//Close input and output files
	fclose(fileIn);
	fclose(fileOut);
	return NOINTENSITIES;
}
