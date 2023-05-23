/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Main function file.
 * 
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Version: 1.1
 * Licensed under GPL-3.0 License
 */ 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
	int optind=2;
	int err;
	char separator='\0';
	
	//Handle CLI switches, print help, GNU Notice...
	err=handleStartup(argc,argv,&optind,&separator);
	if(err!=OK)
	{
		return err;	//don't close files, as they were not opened yet 
	}
	
	FILE* fileIn;
	FILE* fileOut;
	//Open input and output files
	err=openFiles(argc,argv,optind,&fileIn,&fileOut);
	if(err!=OK)
	{
		return err;	//don't close files, as they were not opened yet (or fileIn was closed with openFiles function itself)
	}
	
	uint64_t count=2;
	long double* start=(long double*)malloc(sizeof(long double)*(count));
	
	err=getStartStop(fileIn,&start,&count);
	if(err!=OK)
	{
		fprintf(stderr,"Could not get start or stop angle. Is %s proper input file?\n",argv[argc-optind]);
		return exitProgram(&fileIn,&fileOut,err,start);
	}
	skipHeader(fileIn);
	if(err!=OK)
	{
		return exitProgram(&fileIn,&fileOut,err,start);
	}

	if(count==2)
	{

		long double Dtheta=getDtheta(fileIn, start,&count);
		//fprintf(stderr,"7\n");
		if(Dtheta<=0.0)	//if Dtheta is unreasonable - stop
		{
			return exitProgram(&fileIn,&fileOut,DTHETA,start);
		}
		//fprintf(stderr,"8\n");
		calculateAngles(&start,&count,&Dtheta);
		//fprintf(stderr,"9\n");
	}
	convertData(fileIn,fileOut,separator,start,&count);
	return exitProgram(&fileIn,&fileOut,OK,start);
}
